/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_intersection_utils.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: torinoue <torinoue@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 22:19:33 by torinoue          #+#    #+#             */
/*   Updated: 2025/11/13 14:14:57 by torinoue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

t_map_coord	init_h_intersection_pos(const t_player *player, \
	const t_map_context *map_ctx, const t_raycast *ray)
{
	t_map_coord	pos;

	pos.y = floor(player->pos.y * map_ctx->tile_size_inv)
		* map_ctx->tile_size;
	if (ray->v_direction == DOWN)
		pos.y += map_ctx->tile_size;
	pos.x = player->pos.x + (pos.y - player->pos.y) * ray->tan_inv;
	if (ray->v_direction == UP)
		pos.y -= WALL_OFFSET;
	return (pos);
}
// ft_printf_fd(INFO_FD, "player ");
// print_angle_formatted(player->angle);
// ft_printf_fd(INFO_FD, " ray ");
// print_angle_formatted(ray->angle);
// ft_printf_fd(INFO_FD, " tmp_y:%f	-	pos.y:%f	= %f\n",
// 	tmp_y, pos.y, tmp_y - pos.y);	

t_map_coord	init_v_intersection_pos(const t_player *player, \
	const t_map_context *map_ctx, const t_raycast *ray)
{
	t_map_coord	pos;

	pos.x = floor(player->pos.x * map_ctx->tile_size_inv)
		* map_ctx->tile_size;
	if (ray->h_direction == RIGHT)
		pos.x += map_ctx->tile_size;
	pos.y = ((pos.x - player->pos.x) * ray->tan_val) + player->pos.y;
	if (ray->h_direction == LEFT)
		pos.x -= WALL_OFFSET;
	return (pos);
}

t_map_coord	calc_h_intersection_step(const t_map_context *map_ctx, \
	const t_raycast *ray)
{
	t_map_coord	step;

	step.y = map_ctx->tile_size;
	if (ray->v_direction == UP)
		step.y *= -1;
	step.x = step.y * ray->tan_inv;
	return (step);
}

t_map_coord	calc_v_intersection_step(const t_map_context *map_ctx, \
	const t_raycast *ray)
{
	t_map_coord	step;

	step.x = map_ctx->tile_size;
	if (ray->h_direction == LEFT)
		step.x *= -1;
	step.y = step.x * ray->tan_val;
	return (step);
}
