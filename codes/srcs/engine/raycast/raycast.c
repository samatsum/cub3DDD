/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 13:16:59 by samatsum          #+#    #+#             */
/*   Updated: 2026/06/03 09:21:46 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "types.h"
#include "engine/raycast/raycast.h"

/* ************************************************************************** */
void
	ray_cast(t_camera* camera, t_config* config, t_raysult* ray, double cam_x);
int
	wall_direction(t_raysult* ray);
double
	ray_distance(t_camera* camera, t_raysult* ray);
static void
	init_ray(t_raysult* r, t_camera* c, double camera_x);

/* ************************************************************************** */
// カメラから光線を飛ばし、壁との交点や距離を計算する
void
	ray_cast(t_camera* camera, t_config* config, t_raysult* ray, double cam_x)
{
	int	hit;
	int	next_side;

	copy_pos(&ray->ray_pos, &camera->pos);
	init_ray(ray, camera, cam_x);
	hit = 0;
	while (!hit) {
		next_side = (ray->side_dist.x < ray->side_dist.y);
		ray->side_dist.x += ((next_side) ? ray->delta_dist.x : 0.);
		ray->map_pos.x += ((next_side) ? ray->step.x : 0.);
		ray->side_dist.y += ((!next_side) ? ray->delta_dist.y : 0.);
		ray->map_pos.y += ((!next_side) ? ray->step.y : 0.);
		ray->side = !next_side;
		if (!IN_MAP(ray->map_pos, *config)) {
			ray->map_pos.x -= ((!ray->side) ? ray->step.x : 0.);
			ray->map_pos.y -= ((ray->side) ? ray->step.y : 0.);
			hit = 1;
		} else if (MAP(ray->map_pos, *config) == '1') {
			hit = 1;
		}
	}
	ray->distance = ray_distance(camera, ray);
	ray->direction = wall_direction(ray);
}

/* ************************************************************************** */
// レイが衝突した壁の面に応じてテクスチャの方向を決定する
int
	wall_direction(t_raysult* ray)
{
	if (ray->side) {
		return ((ray->ray_dir.y < 0) ? (TEX_NORTH) : (TEX_SOUTH));
	}
	return ((ray->ray_dir.x < 0) ? (TEX_WEST) : (TEX_EAST));
}

/* ************************************************************************** */
// プレイヤーから壁までの垂直方向の距離を計算する
double
	ray_distance(t_camera* camera, t_raysult* ray)
{
	double	pos;

	if (ray->side) {
		pos = (ray->map_pos.y - camera->pos.y + (1. - ray->step.y) / 2.);
		return (fabs(pos / ray->ray_dir.y));
	}
	pos = (ray->map_pos.x - camera->pos.x + (1. - ray->step.x) / 2.);
	return (fabs(pos / ray->ray_dir.x));
}

/* ************************************************************************** */
// レイの初期化を行い、進行方向とステップサイズを設定する
static void
	init_ray(t_raysult* r, t_camera* c, double camera_x)
{
	set_pos(&r->map_pos, (int)c->pos.x, (int)c->pos.y);
	set_pos(&r->ray_dir, c->dir.x + c->plane.x * camera_x, c->dir.y + c->plane.y * camera_x);
	set_pos(&r->delta_dist, fabs(1. / r->ray_dir.x), fabs(1. / r->ray_dir.y));
	if (r->ray_dir.x < 0.) {
		set_pos(&r->step, -1., (r->ray_dir.y < 0.) ? -1. : 1.);
		r->side_dist.x = (r->ray_pos.x - r->map_pos.x) * r->delta_dist.x;
	} else {
		set_pos(&r->step, 1., (r->ray_dir.y < 0.) ? -1. : 1.);
		r->side_dist.x = (r->map_pos.x + 1. - r->ray_pos.x) * r->delta_dist.x;
	}
	if (r->ray_dir.y < 0.) {
		r->side_dist.y = (r->ray_pos.y - r->map_pos.y) * r->delta_dist.y;
	} else {
		r->side_dist.y = (r->map_pos.y + 1. - r->ray_pos.y) * r->delta_dist.y;
	}
}
