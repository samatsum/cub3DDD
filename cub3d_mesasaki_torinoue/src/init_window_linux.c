/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_window_linux.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: torinoue <torinoue@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 23:57:19 by torinoue          #+#    #+#             */
/*   Updated: 2025/11/06 20:54:30 by torinoue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d_macros.h"
#include "../include/cub3d_types.h"
#include "../lib/ft_printf_fd/ft_printf_fd.h"

#ifndef __APPLE__
# include "../lib/minilibx-linux/mlx.h"

void	limit_window_size_to_screen(void *mlx_ptr, t_win *win)
{
	int	max_width;
	int	max_height;
	int	original_width;
	int	original_height;

	if (!mlx_ptr || !win)
		return ;
	original_width = win->width;
	original_height = win->height;
	mlx_get_screen_size(mlx_ptr, &max_width, &max_height);
	if (max_width > 0 && max_height > 0)
	{
		if (win->width > max_width)
			win->width = max_width;
		if (win->height > max_height)
			win->height = max_height;
		if (original_width != win->width || original_height != win->height)
		{
			ft_printf_fd(INFO_FD,
				"Window size adjusted from %dx%d to %dx%d (max: %dx%d)\n",
				original_width, original_height,
				win->width, win->height,
				max_width, max_height);
		}
	}
}

#endif
