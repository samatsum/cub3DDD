/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 12:33:48 by samatsum          #+#    #+#             */
/*   Updated: 2026/06/03 15:14:34 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/core.h"
#include "engine/texture/texture.h"

/* ************************************************************************** */
int
	shade_color(int color, double divide);
int
	distance_shade(int options, int color, double distance);
int
	get_tex_color(t_tex* tex, t_pos* pos);

/* ************************************************************************** */
// 指定された割合で色を暗くする
int
	shade_color(int color, double divide)
{
	if (divide <= 1.) {
		return (color);
	}
	//RGBの成分を取り出すためのビットシフト
	return (((int)(((0xFF0000 & color) >> 16) / divide) << 16) + ((int)(((0x00FF00 & color) >> 8) / divide) << 8) + ((int)((0x0000FF & color) / divide)));
}

/* ************************************************************************** */
// 距離に応じて色に影を適用する
int
	distance_shade(int options, int color, double distance)
{
	if (options & FLAG_SHADOWS) {
		return (shade_color(color, distance / 1.5));
	}
	return (color);
}

/* ************************************************************************** */
// テクスチャの指定された座標から色を取得する
int
	get_tex_color(t_tex* tex, t_pos* pos)
{
	if (pos->x >= 0 && pos->x < tex->width && pos->y >= 0 && pos->y < tex->height) {
		return (*(int*)(tex->ptr + (4 * tex->width * (int)pos->y) + (4 * (int)pos->x)));
	}
	return (0x0);
}
