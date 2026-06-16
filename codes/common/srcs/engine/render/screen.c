#include <math.h>
#include "core/core.h"            /* ゲーム設定の取得に必要 */
#include "core/respawn.h"         /* is_player_dead に必要 */
#include "engine/raycast/raycast.h"
#include "engine/render/render.h" /* 描画関数群の呼び出しに必要 */
#include "ui/ui.h"                /* update_ui, display_crosshair に必要 */
#include "tuning.h"               /* LIGHT_CONE_DEG に必要 */
#include "../minilibx-linux/mlx.h"

/* ************************************************************************** */

void
	render_frame(t_game* game);
void
	update_screen(t_game* game);
void
	update_window(t_window* w, int options, int collected, int to_collect);
double
	flashlight_weight(t_render* rnd, int column);
static void
	draw_death_screen(t_game* game);

/* ************************************************************************** */

// 1フレーム全体を描画する。死亡中はUIを含む全レイヤーを伏せ、死亡画像だけを表示する
void
	render_frame(t_game* game)
{
	if (is_player_dead(game)) {
		draw_death_screen(game);
		mlx_put_image_to_window(game->window.ptr, game->window.win, game->window.screen.img, 0, 0);
		return ;
	}
	update_screen(game);
	update_window(&game->window, game->options, game->world.collected, game->world.to_collect);
}

/* ************************************************************************** */

// 画面をクリアし、列群を並列レイキャストしてからスプライト・武器・UIを重ねる
void
	update_screen(t_game* game)
{
	t_render	rnd;
	t_window*	w;
	t_pos		start;

	w = &game->window;
	set_pos(&start, 0, 0);
	draw_rectangle(w, &start, &w->size, 0x0);
	rnd.w = w;
	rnd.config = &game->config;
	rnd.camera = &game->camera;
	rnd.world = &game->world;
	rnd.tex = game->assets.tex;
	rnd.door_tex = &game->assets.door_tex;
	rnd.depth = game->cache.depth;
	rnd.sf_dist = game->cache.sf_dist;
	rnd.options = game->options;
	if (game->input.current_weapon == WEP_FLASHLIGHT) {
		rnd.options = rnd.options | FLAG_FLASHLIGHT;
	}
	cast_columns(&rnd, game->cache.camera_x);
	if (game->world.sprites) {
		draw_sprites(&rnd, game->world.sprites);
	}
	draw_weapon(game);
	if (game->options & FLAG_CROSSHAIR) {
		display_crosshair(w);
	}
	if (game->options & FLAG_UI) {
		update_ui(&rnd);
	}
}

/* ************************************************************************** */

// 描画したイメージをウィンドウに反映し、必要に応じてUIテキストを表示する
void
	update_window(t_window* w, int options, int collected, int to_collect)
{
	if (options & FLAG_UI) {
		write_ui_text(w, collected, to_collect);
	}
	mlx_put_image_to_window(w->ptr, w->win, w->screen.img, 0, 0);
}

/* ************************************************************************** */

// 列のレイ角が正面±LIGHT_CONE_DEG度以内なら1.0、端へ向かうほど0に落ちる重み
double
	flashlight_weight(t_render* rnd, int column)
{
	double	camera_x;
	double	plane_len;
	double	dir_len;
	double	angle;
	double	limit;

	if (!(rnd->options & FLAG_FLASHLIGHT)) {
		return (0.0);
	}
	camera_x = 2.0 * column / rnd->w->size.x - 1.0;
	plane_len = hypot(rnd->camera->plane.x, rnd->camera->plane.y);
	dir_len = hypot(rnd->camera->dir.x, rnd->camera->dir.y);
	angle = atan(fabs(camera_x) * plane_len / dir_len);
	limit = LIGHT_CONE_DEG * 0.01745329251994;
	if (angle >= limit) {
		return (0.0);
	}
	return (1.0 - angle / limit);
}

/* ************************************************************************** */

// バッファを黒で塗ってから死亡画像を画面全体へ引き伸ばす（未ロード時は黒画面）
static void
	draw_death_screen(t_game* game)
{
	t_window*	w;
	t_tex*		tex;
	t_pos		pixel;
	t_pos		texel;

	w = &game->window;
	set_pos(&pixel, 0, 0);
	draw_rectangle(w, &pixel, &w->size, 0x000000);
	tex = &game->assets.death_tex;
	if (!tex->tex) {
		return ;
	}
	pixel.y = 0;
	while (pixel.y < w->size.y) {
		texel.y = (int)(pixel.y * tex->height / w->size.y);
		pixel.x = 0;
		while (pixel.x < w->size.x) {
			texel.x = (int)(pixel.x * tex->width / w->size.x);
			draw_pixel(w, &pixel, get_tex_color(tex, &texel));
			pixel.x++;
		}
		pixel.x = 0;
		pixel.y++;
	}
}
