/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: torinoue <torinoue@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 21:03:45 by torinoue          #+#    #+#             */
/*   Updated: 2025/11/03 21:22:50 by torinoue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

bool	is_on_wall(const t_map_context *map_ctx, \
	const t_map_coord *coord)
{
	int	map_x_index;
	int	map_y_index;

	if (coord->x < 0 || coord->y < 0 || coord->x >= map_ctx->max_coord_x || \
		coord->y >= map_ctx->max_coord_y)
		return (true);
	map_y_index = (int)(coord->y * map_ctx->tile_size_inv);
	map_x_index = (int)(coord->x * map_ctx->tile_size_inv);
	if (map_y_index < 0 || map_y_index >= map_ctx->map_y_size)
		return (true);
	if (map_x_index < 0 || map_x_index >= map_ctx->map_x_size[map_y_index])
		return (true);
	return (map_ctx->map[map_y_index][map_x_index] == '1');
}
