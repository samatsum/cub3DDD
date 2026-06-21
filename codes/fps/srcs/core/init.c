#include <sys/time.h>          /* gettimeofday, struct timeval 用 */
#include "engine/raycast/raycast.h"
#include "core/core.h"
#include "enemy/enemy.h"
#include "../minilibx-linux/mlx.h"
#include "tuning.h"
#include "engine/render/light.h"
#include "core/respawn.h"
#include "rsp/rsp_game.h"

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
static int
	setup_rsp_combatants(t_game* game);
static int
	place_combatants(t_game* game, int* pts, int player);
static void
	set_player_rsp(t_game* game, t_spawn_point* sp, t_team team, int hand);
static int
	spawn_rsp_npc(t_game* game, t_spawn_point* sp, t_team team, int hand);

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
	paths[WTEX_FLASHLIGHT] = "textures/arm/Arm_flashlight.xpm";
	paths[WTEX_HAND_LEFT] = "textures/arm/Arm_lefthand.xpm";
	paths[WTEX_HAND_RIGHT] = "textures/arm/Arm_righthand.xpm";
	i = 0;
	while (i < WEAPON_TEX_COUNT) {
		game->assets.weapon_tex[i].path = ft_strdup(paths[i]);
		game->assets.weapon_tex[i].tex = mlx_xpm_file_to_image(game->window.ptr,game->assets.weapon_tex[i].path, &game->assets.weapon_tex[i].width, &game->assets.weapon_tex[i].height);
		if (game->assets.weapon_tex[i].tex) {
			game->assets.weapon_tex[i].ptr = mlx_get_data_addr(game->assets.weapon_tex[i].tex, &game->assets.weapon_tex[i].bpp, &game->assets.weapon_tex[i].size_line, &game->assets.weapon_tex[i].endian);
		}
		i++;
	}
	game->assets.death_tex.path = ft_strdup(DEATH_TEX_PATH);
	game->assets.death_tex.tex = mlx_xpm_file_to_image(game->window.ptr, game->assets.death_tex.path, &game->assets.death_tex.width, &game->assets.death_tex.height);
	if (game->assets.death_tex.tex) {
		game->assets.death_tex.ptr = mlx_get_data_addr(game->assets.death_tex.tex, &game->assets.death_tex.bpp, &game->assets.death_tex.size_line, &game->assets.death_tex.endian);
	}
	game->assets.door_tex.path = ft_strdup(DOOR_TEX_PATH);
	game->assets.door_tex.tex = mlx_xpm_file_to_image(game->window.ptr, game->assets.door_tex.path, &game->assets.door_tex.width, &game->assets.door_tex.height);
	if (game->assets.door_tex.tex) {
		printf("Success to load door texture: %s\n", game->assets.door_tex.path);
		game->assets.door_tex.ptr = mlx_get_data_addr(game->assets.door_tex.tex,&game->assets.door_tex.bpp, &game->assets.door_tex.size_line, &game->assets.door_tex.endian);
	} else {
		printf("Failed to load door texture: %s\n", game->assets.door_tex.path);
	}
	if (!init_enemy_textures(game)) {
		return (exit_error(game, "Error:\nfailed to load enemy textures.\n"));
	}
	if (game->mode == MODE_RSP && !init_hand_textures(game)) {
		return (exit_error(game, "Error:\nfailed to load hand textures.\n"));
	}
	collect_spawns(&game->config);
	save_spawn(game);
	if (!load_textures(&game->window, game->assets.tex, &game->config)) {
		return (exit_error(game, "Error:\nfailed to load texture(s).\n"));
	}
	if (!load_textures(&game->window, game->assets.tex, &game->config)) {
		return (exit_error(game, "Error:\nfailed to load texture(s).\n"));
	}
	if (!find_sprites(game)) {
		return (exit_error(game, "Error:\nfailed to malloc sprites.\n"));
	}
	if (!build_lights(&game->world, &game->config)) {
		return (exit_error(game, "Error:\nfailed to malloc lights.\n"));
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
	struct timeval	tv;
	int				i;

	set_pos(&game->input.move, 0, 0);
	set_pos(&game->input.x_move, 0, 0);
	set_pos(&game->input.rotate, 0, 0);
	game->world.collected = 0;
	game->options = FLAG_UI | FLAG_SHADOWS | FLAG_CROSSHAIR;
	game->last_options = 0;
	game->world.sprites = NULL;
	game->world.enemies = NULL;
	game->world.lights = NULL;
	game->world.light_count = 0;
	game->death_timer = 0.0;
	game->assets.death_tex.tex = NULL;
	game->assets.death_tex.path = NULL;
	gettimeofday(&tv, NULL);
	game->rsp_seed = (unsigned int)(tv.tv_sec ^ tv.tv_usec);
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
// マップ上のスプライトを検索し、オブジェクト(通行不可/通行可/収集)と敵(M)を登録する。
// N/S/E/W のスポーンマスには、チーム色のマーカー(光る装飾)を自動配置する(案Q-1)。
// RSPモードでは最後に setup_rsp_combatants でチーム/手とプレイヤー/NPCを確定する
static int
	find_sprites(t_game* game)
{
	t_pos		pos;
	t_tex*		tex;
	int			i;
	int			j;
	int			slot;
	char		c;
	t_sprite*	new_sprite;

	game->world.sprites = NULL;
	i = 0;
	while (i < game->config.map.rows) {
		j = 0;
		while (j < game->config.map.columns) {
			set_pos(&pos, j + .5, i + .5);
			c = MAP(pos, game->config);
			if (IS_OBJECT(c)) {
				tex = &game->assets.tex[OBJ_SLOT(c)];
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
				add_enemy(&game->world.enemies, new_sprite, (int)game->config.enemy_hp);
			} else if (IS_SPAWN(c)) {
				slot = spawn_marker_slot(c);
				if (slot >= 0) {
					tex = &game->assets.tex[slot];
					if (tex->tex) {
						new_sprite = add_front_sprite(&game->world.sprites, 0., &pos, tex);
						if (!new_sprite) {
							return (0);
						}
					}
				}
			}
			j++;
		}
		i++;
	}
	if (game->mode == MODE_RSP && !setup_rsp_combatants(game)) {
		return (0);
	}
	return (1);
}

/* ************************************************************************** */

// RSPの戦闘員を確定する。赤(N/W)から2地点、青(S/E)から2地点を重複なく選び、
// 各チーム2地点に満たなければ 0。プレイヤーを4地点から1つランダムに選び、残りを
// NPCにする。前提（各チーム2地点以上）が崩れたマップでは生成失敗とする
static int
	setup_rsp_combatants(t_game* game)
{
	int	pts[RSP_COMBATANTS];
	int	n;
	int	player;

	n = pick_spawn_indices(&game->config, RSP_RED_DIRS, &game->rsp_seed, pts, RSP_TEAM_SPAWNS);
	n += pick_spawn_indices(&game->config, RSP_BLUE_DIRS, &game->rsp_seed, pts + n, RSP_TEAM_SPAWNS);
	if (n < RSP_COMBATANTS) {
		return (0);
	}
	player = (int)((unsigned int)rand_r(&game->rsp_seed) % RSP_COMBATANTS);
	return (place_combatants(game, pts, player));
}

/* ************************************************************************** */

// 4地点を順に処理する。前半 RSP_TEAM_SPAWNS 個が赤、後半が青。各員の初期手は
// ランダム。player 番だけプレイヤー（カメラ＋player_rsp）、他は NPC を生成する
static int
	place_combatants(t_game* game, int* pts, int player)
{
	t_team	team;
	int		hand;
	int		i;

	i = 0;
	while (i < RSP_COMBATANTS) {
		team = TEAM_RED;
		if (i >= RSP_TEAM_SPAWNS) {
			team = TEAM_BLUE;
		}
		hand = (int)((unsigned int)rand_r(&game->rsp_seed) % HAND_COUNT);
		if (i == player) {
			set_player_rsp(game, &game->config.spawns[pts[i]], team, hand);
		} else if (!spawn_rsp_npc(game, &game->config.spawns[pts[i]], team, hand)) {
			return (0);
		}
		i++;
	}
	return (1);
}

/* ************************************************************************** */

// プレイヤーを指定スポーンへ配置し、チーム・手・初期リスポーン地点・生存を記録する
static void
	set_player_rsp(t_game* game, t_spawn_point* sp, t_team team, int hand)
{
	apply_spawn(&game->config, &game->camera, sp);
	game->player_rsp.team = team;
	game->player_rsp.hand = (t_hand)hand;
	copy_pos(&game->player_rsp.spawn, &sp->pos);
	game->player_rsp.alive = 1;
}

/* ************************************************************************** */

// NPC1体を生成する。team×hand のハンドテクスチャでスプライトを作り、敵リストへ
// 追加して rsp 状態（チーム・手・初期リスポーン地点・生存）を埋める
static int
	spawn_rsp_npc(t_game* game, t_spawn_point* sp, t_team team, int hand)
{
	t_tex*		tex;
	t_sprite*	sprite;
	t_enemy*	enemy;

	tex = &game->assets.hand_tex[team * HAND_COUNT + hand];
	sprite = add_front_sprite(&game->world.sprites, 0., &sp->pos, tex);
	if (!sprite) {
		return (0);
	}
	enemy = add_enemy(&game->world.enemies, sprite, (int)game->config.enemy_hp);
	if (!enemy) {
		return (0);
	}
	enemy->rsp.team = team;
	enemy->rsp.hand = (t_hand)hand;
	copy_pos(&enemy->rsp.spawn, &sp->pos);
	enemy->rsp.alive = 1;
	return (1);
}
