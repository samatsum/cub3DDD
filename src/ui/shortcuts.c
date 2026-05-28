/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shortcuts.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/31 16:25:20 by samatsum          #+#    #+#             */
/*   Updated: 2026/05/28 20:04:28 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "types.h"
#include "ui/ui.h"
#include "core/core.h" /* screenshot関数などを呼び出すため */

int
	draw_string(t_window *window, t_pos *s_pos, char *str, int color)
{
	return (mlx_string_put(
			window->ptr, window->win,
			s_pos->x, s_pos->y,
			color, str));
}
