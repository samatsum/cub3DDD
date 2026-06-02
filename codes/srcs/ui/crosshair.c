/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   crosshair.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum  <samatsum@student.42.jp   >      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 14:24:58 by samatsum          #+#    #+#             */
/*   Updated: 2026/05/28 23:35:36 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
#include "engine/render/render.h" /* draw_pixel関数のため */
#include "utils/utils.h"          /* set_pos関数のため */

void
	display_crosshair(t_window *w)
{
	t_pos	pixel;
	int		i;

	if (w->size.x < CH_OUTER || w->size.y < CH_OUTER)
		return ;
	i = -CH_LENGTH;
	while (i < CH_LENGTH)
	{
		if (i < -CH_GAP || i > CH_GAP || i == 0)
		{
			set_pos(&pixel, w->half.x + i, w->half.y);
			draw_pixel(w, &pixel, COLOR_CROSSHAIR);
			set_pos(&pixel, w->half.x, w->half.y + i);
			draw_pixel(w, &pixel, COLOR_CROSSHAIR);
		}
		i++;
	}
}
