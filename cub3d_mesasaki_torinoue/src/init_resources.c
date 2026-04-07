/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_resources.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: torinoue <torinoue@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 23:57:19 by torinoue          #+#    #+#             */
/*   Updated: 2025/11/13 15:05:08 by torinoue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d_macros.h"
#include "../include/cub3d_types.h"
#include "../include/cub3d_functions.h"
#include "../lib/ft_printf_fd/ft_printf_fd.h"
#include "../lib/libft/libft.h"
#include <unistd.h>
#include <stdio.h>

#ifdef __APPLE__
# include "../lib/minilibx_opengl_20191021/mlx.h"
#else
# include "../lib/minilibx-linux/mlx.h"
#endif

static void			init_mlx_context(t_data *data);
static const char	*resolve_window_title(const char *exec_name);

void	init_player(t_player *player, t_map_context *map_ctx, t_data *data)
{
	int			y;
	int			x;
	const int	tile_size_half = map_ctx->tile_size / 2;

	y = -1;
	player->fov = DEFAULT_FOV;
	if (player->fov <= 0.0 || player->fov >= PI * 2 || data->win.width <= 0)
		display_error(INVALID_FOV_WIDTH, data);
	player->fov_per_win_width = player->fov / data->win.width;
	while (map_ctx->map[++y])
	{
		x = -1;
		while (map_ctx->map[y][++x])
		{
			if (ft_strchr("NESW", map_ctx->map[y][x]))
			{
				player->angle = 1.0 / 2.0 * PI * (ft_strchr("ESWN",
							map_ctx->map[y][x]) - "ESWN");
				player->pos.x = x * map_ctx->tile_size + tile_size_half;
				player->pos.y = y * map_ctx->tile_size + tile_size_half;
				return ;
			}
		}
	}
	clean_exit(data, PLAYER_NOT_FOUND);
}

static void	init_mlx_context(t_data *data)
{
	if (data->win.width <= 0 || data->win.height <= 0)
		display_error(INVALID_WINDOW_SIZE, data);
	data->mlx = mlx_init();
	if (!data->mlx)
		display_error(MLX_ERROR, data);
	limit_window_size_to_screen(data->mlx, &data->win);
	ft_printf_fd(INFO_FD, "data->win.width = %d\n", data->win.width);
	ft_printf_fd(INFO_FD, "data->win.height = %d\n", data->win.height);
}

void	create_window(t_data *data)
{
	char		*title;
	const char	*base_title;

	init_mlx_context(data);
	base_title = resolve_window_title(data->exec_name);
	ft_printf_fd(INFO_FD, "exec_name: %s\n", base_title);
	title = ft_strdup(base_title);
	if (!title)
		display_error(MLX_ERROR, data);
	data->win.win = mlx_new_window(data->mlx, data->win.width, \
	data->win.height, title);
	free(title);
	if (!data->win.win)
		display_error(MLX_ERROR, data);
}

void	init_screen_buffer(t_data *data)
{
	data->screen.image = mlx_new_image(data->mlx, data->win.width,
			data->win.height);
	if (!data->screen.image)
		display_error(MLX_ERROR, data);
	data->screen.ptr = mlx_get_data_addr(data->screen.image,
			&data->screen.bits_per_pixel, &data->screen.line_length,
			&data->screen.endian);
	if (!data->screen.ptr)
		display_error(MLX_ERROR, data);
	data->screen.bytes_per_pixel = data->screen.bits_per_pixel / 8;
	data->screen.width = data->win.width;
	data->screen.height = data->win.height;
}

static const char	*resolve_window_title(const char *exec_name)
{
	const char	*slash_pos;
	const char	*base_title;

	base_title = exec_name;
	if (base_title)
	{
		slash_pos = ft_strrchr(base_title, '/');
		if (slash_pos && *(slash_pos + 1) != '\0')
			base_title = slash_pos + 1;
	}
	if (!base_title || *base_title == '\0')
		base_title = "cub3D";
	return (base_title);
}
