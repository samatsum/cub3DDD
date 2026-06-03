/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shortcuts.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by samatsum          #+#    #+#             */
/*   Updated: 2026/06/03 15:17:32 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "ui/ui.h"
#include "core/core.h" /* screenshot関数などを呼び出すため */
#include "../minilibx-linux/mlx.h"

/* ************************************************************************** */
int
	draw_string(t_window* window, t_pos* s_pos, char* str, int color);

/* ************************************************************************** */
// 指定された座標に文字列を描画する
int
	draw_string(t_window* window, t_pos* s_pos, char* str, int color)
{
	return (mlx_string_put(
			window->ptr, window->win,
			s_pos->x, s_pos->y,
			color, str));
}
