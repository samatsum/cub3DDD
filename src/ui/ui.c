/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* ui.c                                               :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2019/11/06 14:40:14 by samatsum          #+#    #+#             */
/* Updated: 2026/05/28 00:00:00 by samatsum         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "types.h"
#include "ui/ui.h"
#include "engine/render/engine_render.h" /* draw_rectangle, draw_string等のため */
#include "utils/utils.h"          /* ft_write_str等の文字列操作やset_posのため */

#define SCALE 6

static int
	case_color(t_config *config, t_camera *camera, int x, int y)
{
	char	c;

	c = MAP_XY(x, y, *config);
	if (c == '1' || c == '2')
		return (0x0);
	else if (y == (int)camera->pos.y
			&& x == (int)camera->pos.x)
		return (0x10CC10);
	else if (c == 'A')
		return (0xCCCCCC);
	return (0xFFFFFF);
}

static void
	draw_minimap(t_render *rnd, t_pos *start, t_pos *end)
{
	int			i;
	int			j;
	int			color;

	i = 0;
	while (i < rnd->config->rows)
	{
		j = 0;
		while (j < rnd->config->columns)
		{
			if ((color = case_color(rnd->config, rnd->camera, j, i)) >= 0)
			{
				set_pos(start,
					rnd->w->size.x - (rnd->config->columns * 5) - 5 + (j * 5),
					rnd->w->size.y - (rnd->config->rows * 5) - 5 + (i * 5));
				set_pos(end,
					rnd->w->size.x - (rnd->config->columns * 5) + (j * 5),
					rnd->w->size.y - (rnd->config->rows * 5) + (i * 5));
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

	set_pos(&start, 2, rnd->w->size.y - 27);
	set_pos(&end, 210, rnd->w->size.y - 2);
	draw_rectangle(rnd->w, &start, &end, 0xFFFFFF);
	draw_minimap(rnd, &start, &end);
}

void
	write_ui_text(t_window *w, int collected, int to_collect)
{
	static char	buf[100];
	t_pos		start;
	int			i;

	i = 0;
	while (i < 100)
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
	set_pos(&start, 5, w->size.y - 25);
	draw_string(w, &start, buf, 0x000000);
}
