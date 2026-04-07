/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   closure.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: torinoue <torinoue@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 21:03:45 by torinoue          #+#    #+#             */
/*   Updated: 2025/11/09 18:54:09 by torinoue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>

#include "../../../../include/cub3d_types.h"
#include "../../../../lib/libft/libft.h"

static bool					is_boundary_invalid(const t_map_context *map_ctx, \
	int map_y, int map_x);
static bool					is_adjacent_invalid(const t_map_context *map_ctx, \
	int map_y, int map_x);
static t_reachable_bounds	init_reachable_bounds(void);
static t_reachable_bounds	update_reachable_bounds(t_reachable_bounds bounds, \
					int x, int y, t_map_context *map_ctx);

/*
** Checks if the map is unclosed (not surrounded by walls)
** 
** Note: This function differs from other is_invalid_* functions:
** - Other is_invalid_* functions are read-only (const parameters)
** - This function modifies map_ctx->reachable_bounds and
**   map_ctx->manhattan_distance
** - Therefore, map_ctx parameter is NOT const
**
** @param map_ctx: Map context to validate and update with reachable bounds
** @return: true if map is unclosed, false otherwise
*/
bool	is_invalid_map_unclosed(t_map_context *map_ctx)
{
	int					y;
	int					x;
	t_reachable_bounds	bounds;

	bounds = init_reachable_bounds();
	y = -1;
	while (map_ctx->map[++y])
	{
		x = -1;
		while (map_ctx->map[y][++x])
		{
			if (ft_strchr("NSWE0", map_ctx->map[y][x]))
			{
				if (is_boundary_invalid(map_ctx, y, x)
					|| is_adjacent_invalid(map_ctx, y, x))
					return (true);
				bounds = update_reachable_bounds(bounds, x, y, map_ctx);
			}
		}
	}
	map_ctx->reachable_bounds = bounds;
	return (false);
}

static bool	is_boundary_invalid(const t_map_context *map_ctx, int map_y, \
					int map_x)
{
	if (map_y == 0 && !ft_strchr("1 ", map_ctx->map[map_y][map_x]))
		return (true);
	if (!map_ctx->map[map_y + 1] && !ft_strchr("1 ", \
			map_ctx->map[map_y][map_x]))
		return (true);
	return (false);
}

static bool	is_adjacent_invalid(const t_map_context *map_ctx, int map_y, \
					int map_x)
{
	if (map_y > 0)
	{
		if (map_x >= map_ctx->map_x_size[map_y - 1]
			|| !ft_strchr("NSWE01", map_ctx->map[map_y - 1][map_x]))
			return (true);
	}
	if (map_x > 0 && !ft_strchr("NSWE01", map_ctx->map[map_y][map_x - 1]))
		return (true);
	if (map_ctx->map[map_y + 1])
	{
		if (map_x >= map_ctx->map_x_size[map_y + 1]
			|| !ft_strchr("NSWE01", map_ctx->map[map_y + 1][map_x]))
			return (true);
	}
	if (map_x + 1 >= map_ctx->map_x_size[map_y]
		|| !ft_strchr("NSWE01", map_ctx->map[map_y][map_x + 1]))
		return (true);
	return (false);
}

static t_reachable_bounds	init_reachable_bounds(void)
{
	t_reachable_bounds	bounds;

	bounds.min_x = INT_MAX;
	bounds.max_x = -1;
	bounds.min_y = INT_MAX;
	bounds.max_y = -1;
	return (bounds);
}

static t_reachable_bounds	update_reachable_bounds(t_reachable_bounds bounds, \
					int x, int y, t_map_context *map_ctx)
{
	if (x < bounds.min_x)
		bounds.min_x = x;
	if (x > bounds.max_x)
		bounds.max_x = x;
	if (y < bounds.min_y)
		bounds.min_y = y;
	if (y > bounds.max_y)
		bounds.max_y = y;
	if (bounds.min_x != INT_MAX && bounds.max_x != -1
		&& bounds.min_y != INT_MAX && bounds.max_y != -1)
		map_ctx->manhattan_distance = (bounds.max_x - bounds.min_x) + \
			(bounds.max_y - bounds.min_y);
	return (bounds);
}
/*
	printf("manhattan_distance: %d\n",manhattan_distance);
	printf("bounds: %d, %d, %d, %d\n", bounds.min_x, \
	bounds.max_x, bounds.min_y, bounds.max_y);
*/
/*
python3 -c "print('1\n'*4096, end='')" >> maps/large/h_long_manhattan.cub
*/