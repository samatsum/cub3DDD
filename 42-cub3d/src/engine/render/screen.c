/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   screen.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/06 14:38:10 by samatsum          #+#    #+#             */
/*   Updated: 2026/05/27 19:52:02 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"

void
	update_screen(t_game *game)
{
	t_window	*w;
	int			i;
	t_raysult	ray;
	t_pos		start;

	w = &game->window;
	set_pos(&start, 0, 0);
	draw_rectangle(w, &start, &w->size, 0x0);
	i = 0;
	while (i < w->size.x)
	{
		ray_cast(game, &ray, i);
		game->depth[i] = ray.distance;
		ray.height = fabs(w->size.y / ray.distance);
		draw_wall(game, &ray);
		if (ray.height < game->window.size.y)
			draw_sky_floor(game, &ray);
		i++;
	}
	if (game->sprites)
		draw_sprites(game);
	if (game->options & FLAG_CROSSHAIR)
		display_crosshair(game);
	if (game->options & FLAG_UI)
		update_ui(game);
}

void
	update_window(t_game *game)
{
	t_window	*w;

	w = &game->window;
	mlx_put_image_to_window(w->ptr, w->win, w->screen.img, 0, 0);
	if (game->options & FLAG_UI)
		write_ui_text(game);
}