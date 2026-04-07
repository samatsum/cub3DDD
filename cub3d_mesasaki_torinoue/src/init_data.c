/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: torinoue <torinoue@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 23:56:12 by torinoue          #+#    #+#             */
/*   Updated: 2025/11/13 15:04:40 by torinoue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static void	init_basic_fields(t_data *data, const char *filepath);
static void	init_map_context_fields(t_map_context *map_ctx);
static void	init_textures(t_image *tex);

void	init_data(t_data *data, const char *filepath)
{
	init_basic_fields(data, filepath);
	init_map_context_fields(&data->map_ctx);
	init_textures(data->tex);
}

static void	init_basic_fields(t_data *data, const char *filepath)
{
	double	fov_half_angle;
	double	fov_tan_val;

	data->mlx = NULL;
	data->win.win = NULL;
	data->win.width = WIN_WIDTH;
	data->exec_name = NULL;
	fov_half_angle = DEFAULT_FOV / 2;
	if (DEFAULT_FOV >= PI * 11.0 / 12.0 || DEFAULT_FOV <= 0.0)
		fov_tan_val = tan(PI * 11.0 / 12.0 / 2);
	else
		fov_tan_val = tan(fov_half_angle);
	data->win.width = data->win.width / fov_tan_val;
	data->win.height = WIN_HEIGHT;
	data->screen.image = NULL;
	data->scene_filepath = (char *)filepath;
	data->game.ceiling_color = DEFAULT_CEILING_COLOR;
	data->original_ceiling_color = DEFAULT_CEILING_COLOR;
	data->collision_ceiling_color = rgb_to_hex_with_brightness(
			hex_to_rgb(DEFAULT_CEILING_COLOR),
			ON_WALL_CEILING_BRIGHTNESS);
	data->floor_color = DEFAULT_FLOOR_COLOR;
	data->game.player.is_colliding = false;
}

static void	init_map_context_fields(t_map_context *map_ctx)
{
	map_ctx->map = NULL;
	map_ctx->map_x_size = NULL;
	map_ctx->map_y_size = 0;
	map_ctx->map_x_size_max = 0;
	map_ctx->manhattan_distance = 0;
	map_ctx->reachable_bounds.min_x = INT_MAX;
	map_ctx->reachable_bounds.max_x = -1;
	map_ctx->reachable_bounds.min_y = INT_MAX;
	map_ctx->reachable_bounds.max_y = -1;
	map_ctx->max_coord_x = 0;
	map_ctx->max_coord_y = 0;
	map_ctx->tile_size = DEFAULT_TILE_SIZE;
	if (map_ctx->tile_size <= 0)
	{
		ft_printf_fd(STDERR_FILENO, "Error\n");
		ft_printf_fd(STDERR_FILENO, "Invalid tile_size (%d)\n", \
			map_ctx->tile_size);
		exit(EXIT_FAILURE);
	}
	map_ctx->tile_size_inv = 1.0 / map_ctx->tile_size;
	map_ctx->tan_threshold_min = 0;
	map_ctx->tan_threshold_max = 0;
}

static void	init_textures(t_image *tex)
{
	int	i;

	i = 0;
	while (i < TEXTURES)
	{
		tex[i].path = NULL;
		tex[i].image = NULL;
		i++;
	}
}
