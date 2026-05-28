#include "engine.h"

static void
	init_draw_sky_floor(t_raysult *ray)
{
	if (ray->side == 0 && ray->ray_dir.x > 0)
		set_pos(&ray->floor_wall,
			ray->map_pos.x, ray->map_pos.y + ray->wall_x);
	else if (ray->side == 0 && ray->ray_dir.x < 0)
		set_pos(&ray->floor_wall,
			ray->map_pos.x + 1., ray->map_pos.y + ray->wall_x);
	else if (ray->side && ray->ray_dir.y > 0)
		set_pos(&ray->floor_wall,
			ray->map_pos.x + ray->wall_x, ray->map_pos.y);
	else if (ray->side && ray->ray_dir.y < 0)
		set_pos(&ray->floor_wall,
			ray->map_pos.x + ray->wall_x, ray->map_pos.y + 1.);
}

static void
	draw_sky_pixel(t_render *rnd, t_raysult *ray, t_pos *pixel, t_pos *p_tex)
{
	t_tex	*tex;

	tex = &rnd->tex[TEX_SKY];
	if (!tex->tex)
	{
		draw_pixel(rnd->w, pixel,
			distance_shade(rnd->options, rnd->config->c[TEX_SKY],
			rnd->sf_dist[ray->row]));
	}
	else
	{
		set_pos(p_tex, (int)(ray->c_floor.x * tex->width) % tex->width,
					(int)(ray->c_floor.y * tex->height) % tex->height);
		draw_pixel(rnd->w, pixel,
			distance_shade(rnd->options, get_tex_color(tex, p_tex),
			rnd->sf_dist[ray->row]));
	}
}

static void
	draw_floor_pixel(t_render *rnd, t_raysult *ray, t_pos *pixel, t_pos *p_tex)
{
	t_tex	*tex;

	tex = &rnd->tex[TEX_FLOOR];
	if (!tex->tex)
	{
		draw_pixel(rnd->w, pixel,
			distance_shade(rnd->options, rnd->config->c[TEX_FLOOR],
			rnd->sf_dist[ray->row]));
	}
	else
	{
		set_pos(p_tex, (int)(ray->c_floor.x * tex->width) % tex->width,
					(int)(ray->c_floor.y * tex->height) % tex->height);
		draw_pixel(rnd->w, pixel,
			distance_shade(rnd->options, get_tex_color(tex, p_tex),
			rnd->sf_dist[ray->row]));
	}
}

void
	draw_sky_floor(t_render *rnd, t_raysult *ray)
{
	int		i;
	t_pos	pixel;
	double	weight;
	t_pos	p_tex;

	init_draw_sky_floor(ray);
	pixel.x = ray->column;
	i = rnd->w->half.y + (ray->height / 2.);
	while (i < rnd->w->size.y)
	{
		ray->row = (int)i;
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
