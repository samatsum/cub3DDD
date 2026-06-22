#include <math.h>                 /* hypot, atan, fabs 用 */
#include "core/core.h"            /* t_game 定義のため */
#include "core/respawn.h"         /* is_player_dead に必要 */
#include "engine/raycast/raycast.h"
#include "engine/render/render.h" /* 描画関数群の呼び出しに必要 */
#include "ui/ui.h"                /* update_ui, display_crosshair, write_ui_text に必要 */
#include "tuning.h"               /* LIGHT_CONE_DEG に必要 */
#include "../minilibx-linux/mlx.h" /* mlx_put_image_to_window 用 */

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
// 1フレーム全体を描画する。死亡中(is_player_dead)はワールドもUIも伏せ、死亡画像だけを描いて
// 即ウィンドウへ転送する。通常時は update_screen で世界を組み立て、update_window で反映する
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
// 画面を黒で消去し、描画に必要な状態を t_render スナップショットへ集約してから、列群を並列
// レイキャストする(cast_columns)。その上にスプライト→武器→照準→UIの順で重ねる。
// 懐中電灯を装備中ならフラッシュライトのフラグを立て、各描画の暗化補正に反映させる
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
	rnd.mode = game->mode;
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
// 完成したバッファ画像をウィンドウへ転送する。UIが有効なら、転送前に収集状況のテキストを
// 画面へ書き込んでおく
void
	update_window(t_window* w, int options, int collected, int to_collect)
{
	if (options & FLAG_UI) {
		write_ui_text(w, collected, to_collect);
	}
	mlx_put_image_to_window(w->ptr, w->win, w->screen.img, 0, 0);
}

/* ************************************************************************** */
// 懐中電灯のコーン内での列の重みを返す。列のレイ角が正面±LIGHT_CONE_DEG度以内なら、中心で
// 1.0・端で 0 へ線形に落ちる値。コーン外や懐中電灯OFFなら 0。角度は plane/dir のベクトル長から
// atan で求め、LIGHT_CONE_DEG をラジアンへ換算した limit と比較する
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
// 死亡演出。バッファを黒で塗ってから死亡画像を画面全体へ最近傍補間で引き伸ばす。各画面ピクセル
// (pixel)に対応するテクセル(texel)を比率で求めてサンプリングする。画像未ロード時は黒画面のまま
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
