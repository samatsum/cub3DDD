/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* ui.c                                               :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/12/06 14:40:14 by samatsum          #+#    #+#             */
/* Updated: 2026/05/28 00:00:00 by samatsum         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "types.h"
#include "ui/ui.h"
#include "engine/render/render.h" /* draw_rectangle, draw_string等のため */
#include "utils/utils.h"          /* ft_write_str等の文字列操作やset_posのため */

static int
	case_color(t_config *config, t_camera *camera, int x, int y)
{
	char	c;

	c = MAP_XY(x, y, *config);
	if (c == '1' || c == '2')
		return (COLOR_MINIMAP_WALL);
	else if (y == (int)camera->pos.y
			&& x == (int)camera->pos.x)
		return (COLOR_MINIMAP_BG);
	else if (c == 'A')
		return (COLOR_UI_TEXT);
	return (COLOR_MINIMAP_EMPTY);
}

static void
	draw_minimap(t_render *rnd, t_pos *start, t_pos *end)
{
	int			i;
	int			j;
	int			color;

	i = 0;
	while (i < rnd->config->map.rows)
	{
		j = 0;
		while (j < rnd->config->map.columns)
		{
			if ((color = case_color(rnd->config, rnd->camera, j, i)) >= 0)
			{
				set_pos(start,
					rnd->w->size.x - (rnd->config->map.columns * MAP_TILE_SIZE) - SCALE + (j * MAP_TILE_SIZE),
					rnd->w->size.y - (rnd->config->map.rows * MAP_TILE_SIZE) - SCALE + (i * MAP_TILE_SIZE));
				set_pos(end,
					rnd->w->size.x - (rnd->config->map.columns * MAP_TILE_SIZE) + (j * MAP_TILE_SIZE),
					rnd->w->size.y - (rnd->config->map.rows * MAP_TILE_SIZE) + (i * MAP_TILE_SIZE));
				draw_rectangle(rnd->w, start, end, color);
			}
			j++;
		}
		i++;
	}
}

void
	update_ui(t_render *rnd)
{
	t_pos		start;
	t_pos		end;

	set_pos(&start, UI_BG_X, rnd->w->size.y - UI_BG_Y);
	set_pos(&end, UI_BG_SIZE, rnd->w->size.y - UI_BG_X);
	draw_rectangle(rnd->w, &start, &end, COLOR_UI_BG);
	draw_minimap(rnd, &start, &end);
}

void
	write_ui_text(t_window *w, int collected, int to_collect)
{
	static char	buf[UI_BUF_SIZE];
	t_pos		start;
	int			i;

	i = 0;
	while (i < UI_BUF_SIZE)
		buf[i++] = 0;
	if (to_collect > 0 && to_collect == collected)
		ft_write_str(buf, "GG !", 0);
	else if (to_collect > 0)
	{
		i = ft_write_str(buf, "Collect: ", 0);
		i = ft_write_str(buf, " / ", ft_write_int(buf, collected, i));
		i = ft_write_int(buf, to_collect, i);
	}
	else
		ft_write_str(buf, "Nothing to collect !", 0);
	set_pos(&start, MAP_TILE_SIZE, w->size.y - UI_TEXT_Y);
	draw_string(w, &start, buf, COLOR_UI_FONT);
}
