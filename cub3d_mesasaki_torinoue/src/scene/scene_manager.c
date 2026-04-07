/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_manager.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: torinoue <torinoue@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 16:30:00 by torinoue          #+#    #+#             */
/*   Updated: 2025/11/13 14:13:25 by torinoue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

/*
** Scene management module for orchestrating scene parsing and validation
** This module provides high-level scene management functions that coordinate
** the parsing and validation of scene files (.cub format)
*/

static void	calculate_map_metrics(t_map_context *map_ctx);
static void	print_map(const t_map_context *map_ctx);
static void	print_tan_threshold(const t_map_context *map_ctx);

/*
** Orchestrates the complete scene parsing process
** This function coordinates the validation and parsing of scene settings,
** map loading, and map format validation in the correct order
**
** @param data: Game data structure to populate with scene information
*/
void	parse_scene(t_data *data)
{
	validate_scene_settings(data);
	parse_scene_settings(data);
	load_map(data);
	validate_map_format(data);
	calculate_map_metrics(&data->map_ctx);
	print_tan_threshold(&data->map_ctx);
	return ;
	print_map(&data->map_ctx);
}

/*
** Calculates map-related metrics needed for raycasting and collision detection
** This function computes maximum map width, manhattan distance,
** coordinate bounds, and tangent thresholds based on map dimensions
**
** @param map_ctx: Map context to populate with calculated metrics
*/
static void	calculate_map_metrics(t_map_context *map_ctx)
{
	int	i;

	i = -1;
	map_ctx->map_x_size_max = 0;
	while (map_ctx->map[++i])
		if (map_ctx->map_x_size_max < map_ctx->map_x_size[i])
			map_ctx->map_x_size_max = map_ctx->map_x_size[i];
	if (map_ctx->reachable_bounds.max_x >= 0 && \
		map_ctx->reachable_bounds.max_y >= 0)
	{
		map_ctx->max_coord_x = (map_ctx->reachable_bounds.max_x + 1) * \
		map_ctx->tile_size;
		map_ctx->max_coord_y = (map_ctx->reachable_bounds.max_y + 1) * \
		map_ctx->tile_size;
	}
	else
	{
		map_ctx->max_coord_x = map_ctx->map_x_size_max * map_ctx->tile_size;
		map_ctx->max_coord_y = map_ctx->map_y_size * map_ctx->tile_size;
	}
	map_ctx->tan_threshold_min = (double)map_ctx->tile_size / \
	map_ctx->max_coord_x;
	map_ctx->tan_threshold_max = map_ctx->max_coord_y / \
	(double)map_ctx->tile_size;
}

/*
** Prints the loaded map to standard output
** Private function that outputs each line of the map array
**
** @param map_ctx: Map context containing the map data (const, read-only)
*/
static void	print_map(const t_map_context *map_ctx)
{
	int	i;

	i = 0;
	while (map_ctx->map[i])
	{
		ft_printf_fd(INFO_FD, "%s", map_ctx->map[i]);
		i++;
	}
}

static void	print_tan_threshold(const t_map_context *map_ctx)
{
	ft_printf_fd(INFO_FD, "\n");
	ft_printf_fd(INFO_FD, "max_coord_x: %d\n", \
		(int)map_ctx->max_coord_x);
	ft_printf_fd(INFO_FD, "max_coord_y: %d\n", \
		(int)map_ctx->max_coord_y);
	ft_printf_fd(INFO_FD, "manhattan_distance: %d (%d)\n", \
		map_ctx->manhattan_distance, map_ctx->manhattan_distance * \
		map_ctx->tile_size);
	ft_printf_fd(INFO_FD, "tan_threshold_min: ");
	print_double_formatted_fd(INFO_FD, map_ctx->tan_threshold_min);
	ft_printf_fd(INFO_FD, "\n");
	ft_printf_fd(INFO_FD, "tan_threshold_max: ");
	print_double_formatted_fd(INFO_FD, map_ctx->tan_threshold_max);
	ft_printf_fd(INFO_FD, "\n");
}
