/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_intersection.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: torinoue <torinoue@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 21:03:45 by torinoue          #+#    #+#             */
/*   Updated: 2025/11/13 17:41:55 by torinoue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static t_map_coord	step_to_wall(const t_map_coord *pos, \
	const t_map_coord *step, const t_map_context *map_ctx, \
	const int step_limit);

t_map_coord	get_h_intersection(const t_player *player, \
	const t_map_context *map_ctx, const t_raycast *ray)
{
	t_map_coord	h_start_pos;
	t_map_coord	step;

	h_start_pos = init_h_intersection_pos(player, map_ctx, ray);
	step = calc_h_intersection_step(map_ctx, ray);
	return (step_to_wall(&h_start_pos, &step, map_ctx, map_ctx->map_y_size));
}

t_map_coord	get_v_intersection(const t_player *player, \
	const t_map_context *map_ctx, const t_raycast *ray)
{
	t_map_coord	v_start_pos;
	t_map_coord	step;

	v_start_pos = init_v_intersection_pos(player, map_ctx, ray);
	step = calc_v_intersection_step(map_ctx, ray);
	return (step_to_wall(&v_start_pos, &step, map_ctx, \
		map_ctx->map_x_size_max));
}

static t_map_coord	step_to_wall(const t_map_coord *pos, \
	const t_map_coord *step, const t_map_context *map_ctx, const int step_limit)
{
	t_map_coord	result;
	int			i;

	result = *pos;
	i = 0;
	while (!is_on_wall(map_ctx, &result) && i <= step_limit)
	{
		result.x += step->x;
		result.y += step->y;
		i++;
	}
	return (result);
}
