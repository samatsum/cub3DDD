#include "config/config.h"
#include "engine/render/render.h"
#include "engine/texture/texture.h" /* shade_color 関数を使うため */

/* ************************************************************************** */

void
	draw_sky_floor(t_render* rnd, t_raysult* ray);
static void
	init_draw_sky_floor(t_raysult* ray);
static void
	draw_floor_pixel(t_render* rnd, t_raysult* ray, t_pos* pixel, t_pos* p_tex, double light);
static void
	draw_sky_pixel(t_render* rnd, t_raysult* ray, t_pos* pixel, t_pos* p_tex, double light);

/* ************************************************************************** */

// 天井と床を画面に描画する
void
	draw_sky_floor(t_render* rnd, t_raysult* ray)
{
	t_pos	pixel;
	t_pos	p_tex;
	double	weight;
	double	light;
	int		i;

	init_draw_sky_floor(ray);
	pixel.x = ray->column;
	light = flashlight_weight(rnd, ray->column);
	i = rnd->w->half.y + (ray->height / 2.);
	while (i < rnd->w->size.y) {
		ray->row = i;
		weight = rnd->sf_dist[i] / ray->distance;
		set_pos(&ray->c_floor,
			weight * ray->floor_wall.x + (1. - weight) * rnd->camera->pos.x,
			weight * ray->floor_wall.y + (1. - weight) * rnd->camera->pos.y);
		pixel.y = i;
		draw_floor_pixel(rnd, ray, &pixel, &p_tex, light);
		pixel.y = rnd->w->size.y - i++;
		draw_sky_pixel(rnd, ray, &pixel, &p_tex, light);
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
	draw_floor_pixel(t_render* rnd, t_raysult* ray, t_pos* pixel, t_pos* p_tex, double light)
{
	t_tex*	tex;

	tex = &rnd->tex[TEX_FLOOR];
	if (!tex->tex) {
		draw_pixel(rnd->w, pixel, distance_shade(rnd->options, rnd->config->colors[TEX_FLOOR], rnd->sf_dist[ray->row], light));
	} else {
		/* 修正箇所: 重いモジュロ演算(%)を高速なビット論理積(&)に置き換え */
		set_pos(p_tex,
			((int)(ray->c_floor.x * tex->width)) & (tex->width - 1),
			((int)(ray->c_floor.y * tex->height)) & (tex->height - 1));
		draw_pixel(rnd->w, pixel, distance_shade(rnd->options, get_tex_color(tex, p_tex), rnd->sf_dist[ray->row], light));
	}
}

/* ************************************************************************** */

// 天井の1ピクセルを描画する
static void
	draw_sky_pixel(t_render* rnd, t_raysult* ray, t_pos* pixel, t_pos* p_tex, double light)
{
	t_tex*	tex;

	tex = &rnd->tex[TEX_SKY];
	if (!tex->tex) {
		draw_pixel(rnd->w, pixel, distance_shade(rnd->options, rnd->config->colors[TEX_SKY], rnd->sf_dist[ray->row], light));
	} else {
		/* 修正箇所: 重いモジュロ演算(%)を高速なビット論理積(&)に置き換え */
		set_pos(p_tex,
			((int)(ray->c_floor.x * tex->width)) & (tex->width - 1),
			((int)(ray->c_floor.y * tex->height)) & (tex->height - 1));
		draw_pixel(rnd->w, pixel, distance_shade(rnd->options, get_tex_color(tex, p_tex), rnd->sf_dist[ray->row], light));
	}
}
