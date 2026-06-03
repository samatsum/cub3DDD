/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_sky_floor.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 07:32:33 by samatsum          #+#    #+#             */
/*   Updated: 2026/06/03 15:11:54 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config/config.h"
#include "engine/render/render.h"
#include "engine/texture/texture.h" /* shade_color 関数を使うため */

/* ************************************************************************** */
void
	draw_sky_floor(t_render* rnd, t_raysult* ray);
static void
	init_draw_sky_floor(t_raysult* ray);
static void
	draw_floor_pixel(t_render* rnd, t_raysult* ray, t_pos* pixel, t_pos* p_tex);
static void
	draw_sky_pixel(t_render* rnd, t_raysult* ray, t_pos* pixel, t_pos* p_tex);

/* ************************************************************************** */
// 天井と床を画面に描画する
void
	draw_sky_floor(t_render* rnd, t_raysult* ray)
{
	t_pos	pixel;
	t_pos	p_tex;
	double	weight;
	int		i;

	init_draw_sky_floor(ray);
	pixel.x = ray->column;
	i = rnd->w->half.y + (ray->height / 2.);
	while (i < rnd->w->size.y) {
		ray->row = i;
		weight = rnd->sf_dist[i] / ray->distance;
		set_pos(&ray->c_floor,
			weight * ray->floor_wall.x + (1. - weight) * rnd->camera->pos.x,
			weight * ray->floor_wall.y + (1. - weight) * rnd->camera->pos.y);
		pixel.y = i;
		draw_floor_pixel(rnd, ray, &pixel, &p_tex);
		pixel.y = rnd->w->size.y - i++;
		draw_sky_pixel(rnd, ray, &pixel, &p_tex);
	}
}

/* ************************************************************************** */
// 天井と床の描画のための初期座標を設定する
static void
	init_draw_sky_floor(t_raysult* ray)
{
	if (ray->side == 0 && ray->ray_dir.x > 0) {
		set_pos(&ray->floor_wall, ray->map_pos.x, ray->map_pos.y + ray->wall_x);
	} else if (ray->side == 0 && ray->ray_dir.x < 0) {
		set_pos(&ray->floor_wall, ray->map_pos.x + 1., ray->map_pos.y + ray->wall_x);
	} else if (ray->side && ray->ray_dir.y > 0) {
		set_pos(&ray->floor_wall, ray->map_pos.x + ray->wall_x, ray->map_pos.y);
	} else if (ray->side && ray->ray_dir.y < 0) {
		set_pos(&ray->floor_wall, ray->map_pos.x + ray->wall_x, ray->map_pos.y + 1.);
	}
}

/* ************************************************************************** */
// 床の1ピクセルを描画する
static void
	draw_floor_pixel(t_render* rnd, t_raysult* ray, t_pos* pixel, t_pos* p_tex)
{
	t_tex*	tex;

	tex = &rnd->tex[TEX_FLOOR];
	if (!tex->tex) {
		draw_pixel(rnd->w, pixel, distance_shade(rnd->options, rnd->config->colors[TEX_FLOOR], rnd->sf_dist[ray->row]));
	} else {
		set_pos(p_tex, (int)(ray->c_floor.x * tex->width) % tex->width, (int)(ray->c_floor.y * tex->height) % tex->height);
		draw_pixel(rnd->w, pixel, distance_shade(rnd->options, get_tex_color(tex, p_tex), rnd->sf_dist[ray->row]));
	}
}

/* ************************************************************************** */
// 天井の1ピクセルを描画する
static void
	draw_sky_pixel(t_render* rnd, t_raysult* ray, t_pos* pixel, t_pos* p_tex)
{
	t_tex*	tex;

	tex = &rnd->tex[TEX_SKY];
	if (!tex->tex) {
		draw_pixel(rnd->w, pixel, distance_shade(rnd->options, rnd->config->colors[TEX_SKY], rnd->sf_dist[ray->row]));
	} else {
		set_pos(p_tex, (int)(ray->c_floor.x * tex->width) % tex->width, (int)(ray->c_floor.y * tex->height) % tex->height);
		draw_pixel(rnd->w, pixel, distance_shade(rnd->options, get_tex_color(tex, p_tex), rnd->sf_dist[ray->row]));
	}
}
