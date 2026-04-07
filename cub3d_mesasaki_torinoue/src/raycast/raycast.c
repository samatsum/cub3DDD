/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: torinoue <torinoue@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 21:03:45 by torinoue          #+#    #+#             */
/*   Updated: 2025/11/13 14:21:19 by torinoue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static t_raycast	compute_ray(const t_player *player, \
	const t_map_context *map_ctx, const double ray_angle);
static void			calc_draw_range(const t_win *win, \
	const int tile_size, t_raycast *current_ray);
static void			draw_wall_strip(t_data *data, const int strip_id, \
	t_raycast *current_ray);
static int			get_wall_ind(t_wall_side hit_side, \
	t_direction v_direction, t_direction h_direction);

void	ray_casting(t_data *data)
{
	double		ray_angle;
	int			strip_id;
	t_raycast	current_ray;

	ray_angle = data->game.player.angle - (data->game.player.fov / 2);
	strip_id = 0;
	while (strip_id < data->win.width)
	{
		current_ray = compute_ray(&data->game.player, &data->map_ctx, \
			ray_angle);
		current_ray.distance *= cosf((float)(ray_angle - \
		data->game.player.angle));
		calc_draw_range(&data->win, data->map_ctx.tile_size, &current_ray);
		if (current_ray.wall_height > 0)
			draw_wall_strip(data, strip_id, &current_ray);
		strip_id += 1;
		ray_angle += data->game.player.fov_per_win_width;
	}
}

static t_raycast	compute_ray(const t_player *player, \
	const t_map_context *map_ctx, const double ray_angle)
{
	t_raycast	current_ray;

	setup_ray_params(&current_ray, ray_angle);
	current_ray.distance = compute_ray_distance(&current_ray, player, map_ctx);
	return (current_ray);
}

static void	calc_draw_range(const t_win *win, \
	const int tile_size, t_raycast *current_ray)
{
	double	distance;

	distance = current_ray->distance;
	distance = (distance < 0.1) * 0.1 + (distance >= 0.1) * distance;
	current_ray->wall_height = (double)win->height / distance * \
	((double)tile_size / 2) / tanf((float)(DEFAULT_FOV / 2.0));
	if (current_ray->wall_height <= 0)
	{
		current_ray->wall_top = 0;
		current_ray->wall_bottom = 0;
		current_ray->wall_surplus = 0;
		return ;
	}
	current_ray->wall_top = ((double)win->height - \
	current_ray->wall_height) / 2;
	if (current_ray->wall_top < 0)
		current_ray->wall_top = 0;
	current_ray->wall_bottom = ((double)win->height + \
		current_ray->wall_height) / 2;
	if (current_ray->wall_bottom >= win->height)
		current_ray->wall_bottom = win->height;
	current_ray->wall_surplus = (double)current_ray->wall_height / 2 - \
	(double)win->height / 2;
	if (current_ray->wall_surplus < 0)
		current_ray->wall_surplus = 0;
}

static void	draw_wall_strip(t_data *data, const int strip_id, \
	t_raycast *current_ray)
{
	t_image			*tex;

	if (strip_id < 0 || strip_id >= data->win.width)
		return ;
	if (data->game.player.is_colliding && data->tex[TEX_COLLISION].image)
		tex = &data->tex[TEX_COLLISION];
	else
		tex = &data->tex[get_wall_ind(current_ray->hit_side, \
			current_ray->v_direction, current_ray->h_direction)];
	if (!tex || !tex->ptr || tex->width <= 0 || tex->height <= 0)
		return ;
	current_ray->tex_y_step = (double)tex->height / current_ray->wall_height;
	current_ray->tex_y_pos = (current_ray->wall_top - \
	((double)data->win.height / 2) + ((double)current_ray->wall_height / 2)) \
		* current_ray->tex_y_step;
	render_textured_pixels(data, strip_id, tex, current_ray);
}

static int	get_wall_ind(t_wall_side hit_side, \
	t_direction v_direction, t_direction h_direction)
{
	t_textures	tex_ind;

	if (hit_side == HORIZONTAL && v_direction == UP)
		tex_ind = TEX_NORTH;
	else if (hit_side == HORIZONTAL)
		tex_ind = TEX_SOUTH;
	else if (hit_side == VERTICAL && h_direction == LEFT)
		tex_ind = TEX_WEST;
	else
		tex_ind = TEX_EAST;
	return (tex_ind);
}
