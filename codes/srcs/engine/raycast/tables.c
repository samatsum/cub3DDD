/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tables.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 13:16:59 by samatsum          #+#    #+#             */
/*   Updated: 2026/06/10 08:36:14 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "core/core.h"
#include "engine/raycast/raycast.h"

/* ************************************************************************** */
void
	make_tables(t_game* game);
static void
	calculate_camera_x(double width, double* r);
static void
	calculate_sf_dist(double height, double* r);

/* ************************************************************************** */
// カメラ描画用テーブルや回転用三角関数テーブルを初期化する
void
	make_tables(t_game* game)
{
	calculate_camera_x(game->window.size.x, game->cache.camera_x);
	calculate_sf_dist(game->window.size.y, game->cache.sf_dist);
}

/* ************************************************************************** */
// X座標におけるカメラ平面の比率を事前に計算する
static void
	calculate_camera_x(double width, double* r)
{
	int	i;

	i = 0;
	while (i < width) {
		r[i] = ((2. * (double)i) / width) - 1.;
		i++;
	}
}

/* ************************************************************************** */
// 床と天井の距離計算に必要な比率を事前に計算する
static void
	calculate_sf_dist(double height, double* r)
{
	int	i;

	i = 0;
	while (i < height) {
		r[i] = (height / (2. * (double)i - height));
		i++;
	}
}
