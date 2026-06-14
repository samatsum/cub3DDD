#include "core/core.h"            /* ゲーム設定の取得に必要 */
#include "engine/raycast/raycast.h"
#include "engine/render/render.h" /* 描画関数群の呼び出しに必要 */
#include "ui/ui.h"                /* update_ui, display_crosshair に必要 */
#include "../minilibx-linux/mlx.h"

/* ************************************************************************** */
void
	render_frame(t_game* game);
void
	update_screen(t_game* game);
void
	update_window(t_window* w, int options, int collected, int to_collect);

/* ************************************************************************** */
// ゲームの1フレーム全体を描画し、画面を更新する
void
	render_frame(t_game* game)
{
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
	rnd.depth = game->cache.depth;
	rnd.sf_dist = game->cache.sf_dist;
	rnd.options = game->options;
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
