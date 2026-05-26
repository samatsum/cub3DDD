/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 13:27:03 by hyamamot          #+#    #+#             */
/*   Updated: 2026/02/20 13:27:04 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raycasting.h"

static int	get_direction(t_ray *ray)
{
	if (ray->side == 1)
	{
		if (ray->ray_dir_y > 0)
			return (0);
		return (1);
	}
	if (ray->ray_dir_x > 0)
		return (2);
	return (3);
}

static int	get_tex_x(t_vars *vars, t_ray *ray, int direction)
{
	int	tex_x;

	tex_x = (int)(ray->wall_x * vars->tex[direction].w);
	if (ray->side == 0 && ray->ray_dir_x < 0)
		tex_x = (int)((1 - ray->wall_x) * vars->tex[direction].w);
	if (ray->side == 1 && ray->ray_dir_y > 0)
		tex_x = (int)((1 - ray->wall_x) * vars->tex[direction].w);
	return (tex_x);
}

void	render_texture(t_vars *vars, t_ray *ray, int x)
{
	int				direction;
	int				tex_x;
	int				y;
	int				sy;
	unsigned int	color;

	direction = get_direction(ray);
	tex_x = get_tex_x(vars, ray, direction);
	y = ray->draw_start;
	while (y <= ray->draw_end)
	{
		sy = (y + ray->line_height / 2 - HEIGHT / 2)
			* vars->tex[direction].h / ray->line_height;
		if (sy < 0)
			sy = 0;
		if (sy >= vars->tex[direction].h)
			sy = vars->tex[direction].h - 1;
		color = img_pixel_get(&vars->tex[direction], tex_x, sy);
		img_pixel_put(&vars->img, x, y, color);
		y++;
	}
}

void	render_ceiling_and_floor(t_vars *vars, int x, int line_height)
{
	int	y;
	int	draw_start;
	int	draw_end;

	draw_start = -line_height / 2 + HEIGHT / 2;
	if (draw_start < 0)
		draw_start = 0;
	draw_end = line_height / 2 + HEIGHT / 2;
	if (draw_end >= HEIGHT)
		draw_end = HEIGHT;
	y = 0;
	while (y < draw_start)
	{
		img_pixel_put(&vars->img, x, y, vars->color_code[1]);
		y++;
	}
	y = draw_end;
	while (y < HEIGHT)
	{
		img_pixel_put(&vars->img, x, y, vars->color_code[0]);
		y++;
	}
}
