/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* crosshair.c                                        :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/12/06 13:23:13 by samatsum          #+#    #+#             */
/* Updated: 2026/05/28 14:00:00 by samatsum         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "types.h"
#include "ui/ui.h"
#include "engine/render/engine_render.h" /* draw_pixel関数のため */
#include "utils/utils.h"          /* set_pos関数のため */

void
	display_crosshair(t_window *w)
{
	t_pos	pixel;
	int		i;

	if (w->size.x < 11 || w->size.y < 11)
		return ;
	i = -10;
	while (i < 10)
	{
		if (i < -4 || i > 4 || i == 0)
		{
			set_pos(&pixel, w->half.x + i, w->half.y);
			draw_pixel(w, &pixel, 0x9922FF44);
			set_pos(&pixel, w->half.x, w->half.y + i);
			draw_pixel(w, &pixel, 0x9922FF44);
		}
		i++;
	}
}
