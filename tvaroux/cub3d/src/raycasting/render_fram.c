/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_fram.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 13:26:58 by hyamamot          #+#    #+#             */
/*   Updated: 2026/02/20 13:26:59 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raycasting.h"

static void	ray_set_step(t_vars *vars, t_ray *ray)
{
	if (ray->ray_dir_x < 0)
	{
		ray->step_x = -1;
		ray->side_dist_x = (vars->pos_x - ray->map_x) * ray->delta_dist_x;
	}
	else
	{
		ray->step_x = 1;
		ray->side_dist_x = (ray->map_x + 1 - vars->pos_x) * ray->delta_dist_x;
	}
	if (ray->ray_dir_y < 0)
	{
		ray->step_y = -1;
		ray->side_dist_y = (vars->pos_y - ray->map_y) * ray->delta_dist_y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist_y = (ray->map_y + 1 - vars->pos_y) * ray->delta_dist_y;
	}
}

void	ray_init(t_vars *vars, t_ray *ray, int x)
{
	ray->t = 2 * x / (double)WIDTH - 1;
	ray->ray_dir_x = vars->dir_x + ray->t * vars->plane_x;
	ray->ray_dir_y = vars->dir_y + ray->t * vars->plane_y;
	ray->map_x = (int)vars->pos_x;
	ray->map_y = (int)vars->pos_y;
	if (ray->ray_dir_x == 0)
		ray->delta_dist_x = 1e30;
	else
		ray->delta_dist_x = fabs(1 / ray->ray_dir_x);
	if (ray->ray_dir_y == 0)
		ray->delta_dist_y = 1e30;
	else
		ray->delta_dist_y = fabs(1 / ray->ray_dir_y);
	ray->hit = 0;
	ray_set_step(vars, ray);
}

void	ray_dda(t_vars *vars, t_ray *ray)
{
	while (!ray->hit)
	{
		if (ray->side_dist_x < ray->side_dist_y)
		{
			ray->side_dist_x += ray->delta_dist_x;
			ray->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_dist_y += ray->delta_dist_y;
			ray->map_y += ray->step_y;
			ray->side = 1;
		}
		if (vars->map[ray->map_y][ray->map_x] == 1)
			ray->hit = 1;
	}
}

static void	ray_calc_wall(t_vars *vars, t_ray *ray)
{
	if (ray->side == 0)
		ray->ray_l = ray->side_dist_x - ray->delta_dist_x;
	else
		ray->ray_l = ray->side_dist_y - ray->delta_dist_y;
	if (ray->ray_l < 0.00001)
		ray->ray_l = 0.00001;
	ray->line_height = (int)(HEIGHT / ray->ray_l);
	ray->draw_start = -ray->line_height / 2 + HEIGHT / 2;
	if (ray->draw_start < 0)
		ray->draw_start = 0;
	ray->draw_end = ray->line_height / 2 + HEIGHT / 2;
	if (ray->draw_end >= HEIGHT)
		ray->draw_end = HEIGHT - 1;
	if (ray->side == 0)
		ray->wall_x = vars->pos_y + ray->ray_l * ray->ray_dir_y;
	else
		ray->wall_x = vars->pos_x + ray->ray_l * ray->ray_dir_x;
	ray->wall_x -= floor(ray->wall_x);
}

int	render_fram(t_vars *vars)
{
	int		x;
	t_ray	ray;

	clear_image(&vars->img);
	x = 0;
	while (x < WIDTH)
	{
		ray_init(vars, &ray, x);
		ray_dda(vars, &ray);
		ray_calc_wall(vars, &ray);
		render_texture(vars, &ray, x);
		render_ceiling_and_floor(vars, x, ray.line_height);
		x++;
	}
	get_move_input(vars);
	vars->time_ms = get_time_ms();
	mlx_put_image_to_window(vars->mlx, vars->win, vars->img.img, 0, 0);
	return (0);
}
