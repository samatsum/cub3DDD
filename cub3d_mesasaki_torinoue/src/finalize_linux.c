/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finalize_linux.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: torinoue <torinoue@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 21:03:45 by torinoue          #+#    #+#             */
/*   Updated: 2025/11/03 21:24:17 by torinoue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static void	free_all_vars(t_data *data);
static void	free_textures(t_data *data);
static void	free_mlx_resources(t_data *data);
static void	free_map_data(t_data *data);

void	clean_exit(t_data *data, int exit_status)
{
	free_all_vars(data);
	exit(exit_status);
}

static void	free_all_vars(t_data *data)
{
	free_textures(data);
	free_mlx_resources(data);
	free_map_data(data);
}

static void	free_textures(t_data *data)
{
	int	i;

	i = -1;
	while (++i < TEXTURES)
	{
		free(data->tex[i].path);
		if (data->mlx && data->tex[i].image)
			mlx_destroy_image(data->mlx, data->tex[i].image);
	}
}

static void	free_mlx_resources(t_data *data)
{
	if (data->mlx && data->screen.image)
		mlx_destroy_image(data->mlx, data->screen.image);
	if (data->win.win)
		mlx_destroy_window(data->mlx, data->win.win);
	if (data->mlx)
	{
		mlx_destroy_display(data->mlx);
		free(data->mlx);
	}
}

static void	free_map_data(t_data *data)
{
	int	i;

	if (data->map_ctx.map)
	{
		i = 0;
		while (i <= data->map_ctx.map_y_size)
			free(data->map_ctx.map[i++]);
		free(data->map_ctx.map);
	}
	if (data->map_ctx.map_x_size)
	{
		free(data->map_ctx.map_x_size);
		data->map_ctx.map_x_size = NULL;
	}
}
