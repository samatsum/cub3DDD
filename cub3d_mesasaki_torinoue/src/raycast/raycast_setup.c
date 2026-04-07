/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_setup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: torinoue <torinoue@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 22:19:33 by torinoue          #+#    #+#             */
/*   Updated: 2025/11/13 14:13:38 by torinoue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static void	determine_ray_direction(t_raycast *ray);

void	setup_ray_params(t_raycast *ray, const double ray_angle)
{
	ray->angle = normalize_angle(ray_angle);
	determine_ray_direction(ray);
	ray->tan_val = tanf((float)ray->angle);
}

static void	determine_ray_direction(t_raycast *ray)
{
	if (ray->angle <= PI)
		ray->v_direction = DOWN;
	else
		ray->v_direction = UP;
	if (ray->angle <= PI / 2 || ray->angle > 3 * PI / 2)
		ray->h_direction = RIGHT;
	else
		ray->h_direction = LEFT;
}

// static void	determine_ray_direction(t_raycast *ray)
// {
// 	if (0 < ray->dir_vector_y)
// 		ray->v_direction = DOWN;
// 	else
// 		ray->v_direction = UP;
// 	if (0 < ray->dir_vector_x)
// 		ray->h_direction = RIGHT;
// 	else
// 		ray->h_direction = LEFT;
// }
