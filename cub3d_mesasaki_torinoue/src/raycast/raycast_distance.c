/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_distance.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: torinoue <torinoue@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 18:40:00 by toruinoue         #+#    #+#             */
/*   Updated: 2025/11/13 17:46:30 by torinoue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"
#include <stdio.h>

static void	compute_distances(t_raycast *ray, const t_player *player, \
	const t_map_coord *h_int, const t_map_coord *v_int);

double	compute_ray_distance(t_raycast *ray, \
	const t_player *player, const t_map_context *map_ctx)
{
	t_map_coord	h_int;
	t_map_coord	v_int;

	if (isnan(ray->tan_val))
	{
		ray->intersection = get_v_intersection(player, map_ctx, ray);
		ray->hit_side = VERTICAL;
		ray->distance = fabsf((float)(ray->intersection.x - player->pos.x)) * \
			hypotenuse_unit(ray->tan_val);
		return (ray->distance);
	}
	ray->tan_inv = safe_tan_inverse(ray->tan_val);
	v_int = get_v_intersection(player, map_ctx, ray);
	h_int = get_h_intersection(player, map_ctx, ray);
	compute_distances(ray, player, &h_int, &v_int);
	return (ray->distance);
}

// static void	compute_distances(t_raycast *ray, const t_player *player, \
// 	const t_map_coord *h_int, const t_map_coord *v_int)
// {
// 	double	h_dist;
// 	double	v_dist;

// 	h_dist = fabsf((float)(h_int->y - player->pos.y)) * \
// 		hypotenuse_unit(ray->tan_inv);
// 	v_dist = fabsf((float)(v_int->x - player->pos.x)) * \
// 		hypotenuse_unit(ray->tan_val);
// 	if (h_dist < v_dist)
// 	{
// 		ray->intersection = *h_int;
// 		ray->hit_side = HORIZONTAL;
// 		ray->distance = h_dist;
// 		if ( 5.49 < ray->angle &&  ray->angle < 5.5)
// 		{
// 		printf("h_dist%f\n", v_dist);
// 			sleep(1);
// 		}
// 	}
// 	else
// 	{
// 		printf("v_dist%f\n", v_dist);
// 		ray->intersection = *v_int;
// 		ray->hit_side = VERTICAL;
// 		ray->distance = v_dist;
// 	}
// }
static void	compute_distances(t_raycast *ray, const t_player *player, \
	const t_map_coord *h_int, const t_map_coord *v_int)
{
	double	delta_x_to_vertical_wall;
	double	delta_y_to_horizontal_wall;	

	delta_x_to_vertical_wall = fabs(v_int->x - player->pos.x);
	delta_y_to_horizontal_wall = fabs(h_int->y - player->pos.y);
	if ((delta_x_to_vertical_wall == delta_y_to_horizontal_wall && \
	fabs(ray->tan_val) < 1) || (delta_x_to_vertical_wall * \
	fabs(ray->tan_val)) < delta_y_to_horizontal_wall)
	{
		ray->intersection = *v_int;
		ray->hit_side = VERTICAL;
		ray->distance = delta_x_to_vertical_wall * \
		hypotenuse_unit(ray->tan_val);
	}
	else
	{
		ray->intersection = *h_int;
		ray->hit_side = HORIZONTAL;
		ray->distance = delta_y_to_horizontal_wall * \
		hypotenuse_unit(ray->tan_inv);
	}
}
