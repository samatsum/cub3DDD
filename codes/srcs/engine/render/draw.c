/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 00:05:58 by samatsum          #+#    #+#             */
/*   Updated: 2026/06/03 09:27:24 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "types.h"                 /* t_window や t_pos の構造体定義 */
#include "engine/render/render.h"  /* 自身の関数宣言 (draw_rectangle 等) */

/* ************************************************************************** */
void
	draw_pixel(t_window* w, t_pos* pos, int color);
int
	draw_vertical_line(t_window* window, t_pos* start, int length, int color);
int
	draw_rectangle(t_window* window, t_pos* p1, t_pos* p2, int color);
static void
	restrain_pos(t_pos* pos, t_pos* size);

/* ************************************************************************** */
// 画面上の指定された座標に1ピクセルを描画する
void
	draw_pixel(t_window* w, t_pos* pos, int color)
{
	int	offset;

	if (pos->x >= 0 && pos->x < w->size.x && pos->y >= 0 && pos->y < w->size.y) {
		/* 修正: マジックナンバーを排除し、構造体の size_line と bpp を用いて計算 */
		//bpp は「Bits Per Pixel（1ピクセルあたりのビット数）」である。
		//通常は 32（32ビットカラー）が返ってくる。
		//メモリアドレスは「バイト（8ビット）」単位で計算しなければならないため、32 / 8 = 4バイト
		offset = ((int)pos->y * w->screen.size_line) + ((int)pos->x * (w->screen.bpp / 8));
		*(int*)(w->screen.ptr + offset) = color;
	}
}

/* ************************************************************************** */
// 指定された座標から垂直方向に直線を描画する
int
	draw_vertical_line(t_window* window, t_pos* start, int length, int color)
{
	t_pos	pos;
	int		limit;
	int		i;
	int		j;

	if (start->x < 0 || start->x > window->size.x) {
		return (1);
	}
	pos.x = start->x;
	limit = (int)window->size.y;
	i = 0;
	while (i < length && (j = start->y + i) < limit) {
		pos.y = j;
		draw_pixel(window, &pos, color);
		i++;
	}
	return (1);
}

/* ************************************************************************** */
// 指定された2点間の領域を塗りつぶして矩形を描画する
int
	draw_rectangle(t_window* window, t_pos* p1, t_pos* p2, int color)
{
	t_pos	pos;
	int		i;
	int		j;

	restrain_pos(p1, &window->size);
	restrain_pos(p2, &window->size);
	i = p1->y;
	while (i < p2->y) {
		pos.y = i;
		j = p1->x;
		while (j < p2->x) {
			pos.x = j++;
			draw_pixel(window, &pos, color);
		}
		i++;
	}
	return (1);
}

/* ************************************************************************** */
// 座標が画面サイズを超えないように制限をかける
static void
	restrain_pos(t_pos* pos, t_pos* size)
{
	if (pos->x < 0) {
		pos->x = 0;
	}
	if (pos->x > size->x) {
		pos->x = size->x - 1;
	}
	if (pos->y < 0) {
		pos->y = 0;
	}
	if (pos->y > size->y) {
		pos->y = size->y - 1;
	}
}
