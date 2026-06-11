/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 07:32:33 by samatsum          #+#    #+#             */
/*   Updated: 2026/06/11 17:36:45 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine/raycast/raycast.h"
#include "config/config.h" /* MAP等のマクロ展開のため */
#include <math.h>          /* cos, sin 関数用 */

/* ************************************************************************** */
void
	find_start_pos(t_config* config, t_camera* camera);
void
	find_start_angle(t_config* config, t_camera* camera);
int
	move_camera(t_camera* c, t_config* config, int direction, double time_mult);
int
	move_perp_camera(t_camera* c, t_config* config, int direction, double time_mult);
int
	rotate_camera(t_camera* c, t_config* config, int dir, double time_mult);

/* ************************************************************************** */
// プレイヤーの初期スポーン地点を探索し、設定する
void
	find_start_pos(t_config* config, t_camera* camera)
{
	int	stop;

	stop = 0;
	camera->pos.y = 0;
	while (!stop && camera->pos.y < config->map.rows) {
		camera->pos.x = 0;
		while (!stop && camera->pos.x < config->map.columns) {
			if (ft_in_set(MAP(camera->pos, *config), DIRECTIONS)) {
				stop = 1;
				break;
			}
			(camera->pos.x)++;
		}
		if (!stop) {
			(camera->pos.y)++;
		}
	}
	camera->pos.x += .5;
	camera->pos.y += .5;
}

/* ************************************************************************** */
// スポーン地点の方向文字から初期の視界角度とカメラの平面を設定する
void
	find_start_angle(t_config* config, t_camera* camera)
{
	if (MAP(camera->pos, *config) == 'N') {
		set_pos(&camera->dir, 0., -1.);
		set_pos(&camera->plane, config->fov, 0.);
	} else if (MAP(camera->pos, *config) == 'E') {
		set_pos(&camera->dir, 1., 0.);
		set_pos(&camera->plane, 0., config->fov);
	} else if (MAP(camera->pos, *config) == 'S') {
		set_pos(&camera->dir, 0., 1.);
		set_pos(&camera->plane, -config->fov, 0.);
	} else if (MAP(camera->pos, *config) == 'W') {
		set_pos(&camera->dir, -1., 0.);
		set_pos(&camera->plane, 0., -config->fov);
	}
	set_pos(&camera->x_dir, camera->dir.y, -camera->dir.x);
	MAP(camera->pos, *config) = 'A';
}

/* ************************************************************************** */
// 前後へのカメラの移動を行い、壁の判定と訪問済みのマーキングをする
int
	move_camera(t_camera* c, t_config* config, int direction, double time_mult)
{
	t_pos	n_pos;
	double	actual_speed;

	/* 修正: 速度に時間倍率を掛けて、FPSに依存しない移動量を計算 */
	actual_speed = config->move_speed  * time_mult;
	copy_pos(&n_pos, &c->pos);
	n_pos.x += (((direction) ? -1 : 1) * (c->dir.x * actual_speed));
	if (IN_MAP(n_pos, *config) && MAP(n_pos, *config) != '1' && MAP(n_pos, *config) != '2') {
		copy_pos(&c->pos, &n_pos);
	}
	copy_pos(&n_pos, &c->pos);
	n_pos.y += (((direction) ? -1 : 1) * (c->dir.y * actual_speed));
	if (IN_MAP(n_pos, *config) && MAP(n_pos, *config) != '1' && MAP(n_pos, *config) != '2') {
		copy_pos(&c->pos, &n_pos);
	}
	if (MAP(c->pos, *config) != '4') {
		MAP(c->pos, *config) = 'A';
	}
	return (1);
}

/* ************************************************************************** */
// 左右へのカメラの平行移動を行い、壁の判定と訪問済みのマーキングをする
int
	move_perp_camera(t_camera* c, t_config* config, int direction, double time_mult)
{
	t_pos	n_pos;
	double	actual_speed;

	/* 修正: 速度に時間倍率を掛けて、FPSに依存しない移動量を計算 */
	actual_speed = config->move_speed * time_mult;
	copy_pos(&n_pos, &c->pos);
	n_pos.x += (((direction) ? -1 : 1) * (c->x_dir.x * actual_speed) + COLLISION_MARGIN);
	if (IN_MAP(n_pos, *config) && MAP(n_pos, *config) != '1' && MAP(n_pos, *config) != '2') {
		copy_pos(&c->pos, &n_pos);
	}
	copy_pos(&n_pos, &c->pos);
	n_pos.y += (((direction) ? -1 : 1) * (c->x_dir.y * actual_speed) + COLLISION_MARGIN);
	if (IN_MAP(n_pos, *config) && MAP(n_pos, *config) != '1' && MAP(n_pos, *config) != '2') {
		copy_pos(&c->pos, &n_pos);
	}
	if (MAP(c->pos, *config) != '4') {
		MAP(c->pos, *config) = 'A';
	}
	return (1);
}

/* ************************************************************************** */
// カメラの視線を回転させ、方向ベクトルと平面ベクトルを更新する
int
	rotate_camera(t_camera* c, t_config* config, int dir, double time_mult)
{
	t_pos	old;
	double	actual_rot;
	double	cos_val;
	double	sin_val;

	/* 修正: 回転速度に時間倍率を掛け、可変FPSに対応するため毎回cos/sinを計算する */
	actual_rot = config->rotate_speed * time_mult;
	if (dir == 0) {
		actual_rot = -actual_rot;
	}
	cos_val = cos(actual_rot);
	sin_val = sin(actual_rot);

	copy_pos(&old, &c->dir);
	c->dir.x = (c->dir.x * cos_val) - (c->dir.y * sin_val);
	c->dir.y = (old.x * sin_val) + (c->dir.y * cos_val);
	copy_pos(&old, &c->plane);
	c->plane.x = (c->plane.x * cos_val) - (c->plane.y * sin_val);
	c->plane.y = (old.x * sin_val) + (c->plane.y * cos_val);
	copy_pos(&old, &c->x_dir);
	c->x_dir.x = (c->x_dir.x * cos_val) - (c->x_dir.y * sin_val);
	c->x_dir.y = (old.x * sin_val) + (c->x_dir.y * cos_val);
	return (1);
}
