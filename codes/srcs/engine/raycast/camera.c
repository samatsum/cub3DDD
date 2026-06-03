/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 07:32:33 by samatsum          #+#    #+#             */
/*   Updated: 2026/06/03 09:21:20 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "types.h"
#include "engine/raycast/raycast.h"
#include "utils/utils.h" /* set_pos関数を使うため */

/* ************************************************************************** */
void
	find_start_pos(t_config* config, t_camera* camera);
void
	find_start_angle(t_config* config, t_camera* camera);
int
	move_camera(t_camera* c, t_config* config, int direction);
int
	move_perp_camera(t_camera* c, t_config* config, int direction);
int
	rotate_camera(t_camera* c, double* cos_arr, double* sin_arr, int dir);

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
	/* 修正: 初期スポーン地点を '0' ではなく 'A' (訪問済み) にする */
	MAP(camera->pos, *config) = 'A';
}

/* ************************************************************************** */
// 前後へのカメラの移動を行い、壁の判定と訪問済みのマーキングをする
int
	move_camera(t_camera* c, t_config* config, int direction)
{
	t_pos	n_pos;

	copy_pos(&n_pos, &c->pos);
	n_pos.x += (((direction) ? -1 : 1) * (c->dir.x * MOVE_SPEED));
	if (IN_MAP(n_pos, *config) && MAP(n_pos, *config) != '1' && MAP(n_pos, *config) != '2') {
		copy_pos(&c->pos, &n_pos);
	}
	copy_pos(&n_pos, &c->pos);
	n_pos.y += (((direction) ? -1 : 1) * (c->dir.y * MOVE_SPEED));
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
	move_perp_camera(t_camera* c, t_config* config, int direction)
{
	t_pos	n_pos;

	copy_pos(&n_pos, &c->pos);
	n_pos.x += (((direction) ? -1 : 1) * (c->x_dir.x * MOVE_SPEED) + COLLISION_MARGIN);
	if (IN_MAP(n_pos, *config) && MAP(n_pos, *config) != '1' && MAP(n_pos, *config) != '2') {
		copy_pos(&c->pos, &n_pos);
	}
	copy_pos(&n_pos, &c->pos);
	n_pos.y += (((direction) ? -1 : 1) * (c->x_dir.y * MOVE_SPEED) + COLLISION_MARGIN);
	if (IN_MAP(n_pos, *config) && MAP(n_pos, *config) != '1' && MAP(n_pos, *config) != '2') {
		copy_pos(&c->pos, &n_pos);
	}
	/* 追加: 横移動が完了した現在地を 'A' (訪問済み) でマーキング */
	if (MAP(c->pos, *config) != '4') {
		MAP(c->pos, *config) = 'A';
	}
	return (1);
}

/* ************************************************************************** */
// カメラの視線を回転させ、方向ベクトルと平面ベクトルを更新する
int
	rotate_camera(t_camera* c, double* cos_arr, double* sin_arr, int dir)
{
	t_pos	old;

	copy_pos(&old, &c->dir);
	c->dir.x = (c->dir.x * cos_arr[dir]) - (c->dir.y * sin_arr[dir]);
	c->dir.y = (old.x * sin_arr[dir]) + (c->dir.y * cos_arr[dir]);
	copy_pos(&old, &c->plane);
	c->plane.x = (c->plane.x * cos_arr[dir]) - (c->plane.y * sin_arr[dir]);
	c->plane.y = (old.x * sin_arr[dir]) + (c->plane.y * cos_arr[dir]);
	copy_pos(&old, &c->x_dir);
	c->x_dir.x = (c->x_dir.x * cos_arr[dir]) - (c->x_dir.y * sin_arr[dir]);
	c->x_dir.y = (old.x * sin_arr[dir]) + (c->x_dir.y * cos_arr[dir]);
	return (1);
}
