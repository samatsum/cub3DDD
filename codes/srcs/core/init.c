#include "engine/raycast/raycast.h"
#include "core/core.h"
#include "enemy/enemy.h"
#include "../minilibx-linux/mlx.h"
#include "tuning.h"

/* ************************************************************************** */
int
	finish_init(t_game* game);
void
	init_game(t_game* game);
int
	init_window(t_window* window, t_config* config);
int
	init_image(t_window* window, t_image* img);
static int
	find_sprites(t_game* game);

/* ************************************************************************** */
// ゲームの初期化処理を完了させ、必要なリソースを準備する
int
	finish_init(t_game* game)
{
	int			i;
	const char*	paths[WEAPON_TEX_COUNT];

	if (!init_window(&game->window, &game->config)) {
		return (exit_error(game, "Error:\nmlx failed to create window or image.\n"));
	}
	game->input.current_weapon = WEP_PISTOL;
	game->input.is_shooting = 0;
	paths[WTEX_PISTOL_IDLE] = "textures/arm/Arm_pistol_static.xpm";
	paths[WTEX_PISTOL_SHOOT] = "textures/arm/Arm_pistol_shoot.xpm";
	paths[WTEX_PISTOL_RECOIL] = "textures/arm/Arm_pistol_recoil.xpm";
	paths[WTEX_FLASHLIGHT] = "textures/arm/Arm_flashlight_1.xpm";
	paths[WTEX_HAND_LEFT] = "textures/arm/Arm_lefthand.xpm";
	paths[WTEX_HAND_RIGHT] = "textures/arm/Arm_righthand.xpm";
	i = 0;
	while (i < WEAPON_TEX_COUNT) {
		game->assets.weapon_tex[i].path = ft_strdup(paths[i]);
		game->assets.weapon_tex[i].tex = mlx_xpm_file_to_image(game->window.ptr, game->assets.weapon_tex[i].path, &game->assets.weapon_tex[i].width, &game->assets.weapon_tex[i].height);
		if (game->assets.weapon_tex[i].tex) {
			game->assets.weapon_tex[i].ptr = mlx_get_data_addr(game->assets.weapon_tex[i].tex, &game->assets.weapon_tex[i].bpp, &game->assets.weapon_tex[i].size_line, &game->assets.weapon_tex[i].endian);
		}
		i++;
	}
	if (!init_enemy_textures(game)) {
		return (exit_error(game, "Error:\nfailed to load enemy textures.\n"));
	}
	find_start_pos(&game->config, &game->camera);
	find_start_angle(&game->config, &game->camera);
	if (!load_textures(&game->window, game->assets.tex, &game->config)) {
		return (exit_error(game, "Error:\nfailed to load texture(s).\n"));
	}
	if (!find_sprites(game)) {
		return (exit_error(game, "Error:\nfailed to malloc sprites.\n"));
	}
	count_items(game);
	make_tables(game);
	return (1);
}

/* ************************************************************************** */
// ゲームの内部状態を初期化する
void
	init_game(t_game* game)
{
	int	i;

	set_pos(&game->input.move, 0, 0);
	set_pos(&game->input.x_move, 0, 0);
	set_pos(&game->input.rotate, 0, 0);
	game->world.collected = 0;
	game->options = FLAG_UI | FLAG_SHADOWS | FLAG_CROSSHAIR;
	game->last_options = 0;
	game->world.sprites = NULL;
	game->world.enemies = NULL;
	i = 0;
	while (i < TEXTURES) {
		game->assets.tex[i++].tex = NULL;
	}
}

/* ************************************************************************** */
// ウィンドウを作成し、画面サイズやFOVの設定を行う
int
	init_window(t_window* window, t_config* config)
{
	set_pos(&window->size, config->requested_width, config->requested_height);
	if (window->size.x > MAX_WIDTH) {
		window->size.x = MAX_WIDTH;
	}
	if (window->size.y > MAX_HEIGHT) {
		window->size.y = MAX_HEIGHT;
	}
	if (window->size.x < MIN_WIDTH) {
		window->size.x = MIN_WIDTH;
	}
	if (window->size.y < MIN_HEIGHT) {
		window->size.y = MIN_HEIGHT;
	}
	window->ptr = NULL;
	window->win = NULL;
	window->ratio = window->size.x / window->size.y;
	window->screen.img = NULL;
	if (window->ratio < BEST_RATIO) {
		config->fov = config->fov / ((BEST_RATIO / config->fov) / FOV_SCALE);
	} else if (window->ratio > BEST_RATIO) {
		config->fov = config->fov * ((config->fov / BEST_RATIO) * FOV_SCALE);
	}
	window->ptr = mlx_init();
	if (!window->ptr) {
		return (0);
	}
	window->win = mlx_new_window(window->ptr, window->size.x, window->size.y, "cub3d");
	if (!window->win) {
		return (0);
	}
	set_pos(&window->half, window->size.x / 2, window->size.y / 2);
	if (!init_image(window, &window->screen)) {
		return (0);
	}
	return (1);
}

/* ************************************************************************** */
// メモリ上に描画用のイメージ領域を確保し、データアドレスを取得する
int
	init_image(t_window* window, t_image* img)
{
	img->img = mlx_new_image(window->ptr, window->size.x, window->size.y);
	if (!img->img) {
		return (0);
	}
	img->ptr = mlx_get_data_addr(img->img, &img->bpp, &img->size_line, &img->endian);
	return (1);
}

/* ************************************************************************** */
// マップ上のスプライトを検索し、オブジェクト(2-4)と敵(M)をそれぞれ登録する
static int
	find_sprites(t_game* game)
{
	t_pos		pos;
	t_tex*		tex;
	int			i;
	int			j;
	char		c;
	t_sprite*	new_sprite;

	game->world.sprites = NULL;
	i = 0;
	while (i < game->config.map.rows) {
		j = 0;
		while (j < game->config.map.columns) {
			set_pos(&pos, j + .5, i + .5);
			c = MAP(pos, game->config);
			if (c >= TILE_OBSTACLE && c <= TILE_ITEM) {
				tex = &game->assets.tex[TEX_SPRITE + (c - TILE_OBSTACLE)];
				if (tex->tex) {
					new_sprite = add_front_sprite(&game->world.sprites, 0., &pos, tex);
					if (!new_sprite) {
						return (0);
					}
				}
			} else if (c == 'M') {
				tex = &game->assets.enemy_tex[0];
				new_sprite = add_front_sprite(&game->world.sprites, 0., &pos, tex);
				if (!new_sprite) {
					return (0);
				}
				add_enemy(&game->world.enemies, new_sprite);
			}
			j++;
		}
		i++;
	}
	return (1);
}
