#include "types.h"
#include "engine/raycast/raycast.h"
#include "engine/render/render.h" /* 描画関数群の呼び出しに必要 */
#include "ui/ui.h"                /* update_ui, display_crosshair に必要 */

void
	update_screen(t_game *game)
{
	t_window	*w;
	int			i;
	t_raysult	ray;
	t_pos		start;
	t_render	rnd; /* 新設: 描画コンテキスト */

	w = &game->window;
	set_pos(&start, 0, 0);
	draw_rectangle(w, &start, &w->size, 0x0);
	
	/* 描画専用コンテキストの初期化（God Objectからの切り離し） */
	rnd.w = w;
	rnd.config = &game->config;
	rnd.camera = &game->camera;
	rnd.tex = game->tex;
	rnd.depth = game->depth;
	rnd.sf_dist = game->sf_dist;
	rnd.options = game->options;

	i = 0;
	while (i < w->size.x)
	{
		ray.column = i;
		ray_cast(&game->camera, &game->config, &ray, game->camera_x[i]);
		game->depth[i] = ray.distance;
		ray.height = fabs(w->size.y / ray.distance);
		draw_wall(&rnd, &ray);
		if (ray.height < game->window.size.y)
			draw_sky_floor(&rnd, &ray);
		i++;
	}
	if (game->sprites)
		draw_sprites(&rnd, game->sprites);
	if (game->options & FLAG_CROSSHAIR)
		display_crosshair(w);
	if (game->options & FLAG_UI)
		update_ui(&rnd);
}

void
	update_window(t_window *w, int options, int collected, int to_collect)
{
	mlx_put_image_to_window(w->ptr, w->win, w->screen.img, 0, 0);
	if (options & FLAG_UI)
		write_ui_text(w, collected, to_collect);
}

void render_frame(t_game *game)
{
	update_screen(game);
	update_window(&game->window, game->options, game->collected, game->to_collect);
}
