/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: torinoue <torinoue@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 19:15:06 by torinoue          #+#    #+#             */
/*   Updated: 2025/11/13 15:02:24 by torinoue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d_macros.h"
#include "../include/cub3d_types.h"
#include "../include/cub3d_functions.h"
#include "../lib/libft/libft.h"
#include "../lib/ft_printf_fd/ft_printf_fd.h"
#ifdef __APPLE__
# include "../lib/minilibx_opengl_20191021/mlx.h"
# include "../include/cub3d_keymap_MAC.h"
#else
# include "../lib/minilibx-linux/mlx.h"
# include "../include/cub3d_keymap_linux.h"
#endif
#include <stdlib.h>

static bool	is_command_line_arg_error(int argc, char *argv[]);
static int	main_loop(t_data *data);
static void	setup_event_hooks(t_data *data);

int	main(int argc, char *argv[])
{
	t_data	data;

	if (is_command_line_arg_error(argc, argv))
	{
		ft_printf_fd(STDERR_FILENO, "usage: %s <.cub filepath>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	init_data(&data, argv[1]);
	data.exec_name = argv[0];
	ft_printf_fd(INFO_FD, "exec_name: %s\n", data.exec_name);
	parse_scene(&data);
	create_window(&data);
	init_player(&data.game.player, &data.map_ctx, &data);
	init_screen_buffer(&data);
	load_textures(&data);
	setup_event_hooks(&data);
	mlx_loop(data.mlx);
	return (EXIT_FAILURE);
}

static bool	is_command_line_arg_error(int argc, char *argv[])
{
	return (argc != 2 || !argv[1] || ft_strlen(argv[1]) < 4
		|| ft_strncmp(argv[1] + ft_strlen(argv[1]) - 4, ".cub", 4) != 0);
}

static int	main_loop(t_data *data)
{
	int	ceiling_color;

	data->screen.ptr = mlx_get_data_addr(data->screen.image,
			&data->screen.bits_per_pixel, &data->screen.line_length,
			&data->screen.endian);
	ceiling_color = data->original_ceiling_color;
	if (data->game.player.is_colliding)
		ceiling_color = data->collision_ceiling_color;
	draw_background(&data->screen, &data->win, ceiling_color, \
		data->floor_color);
	ray_casting(data);
	mlx_put_image_to_window(data->mlx, data->win.win, data->screen.image, \
		0, 0);
	return (0);
}

static void	setup_event_hooks(t_data *data)
{
	mlx_hook(data->win.win, ON_KEYDOWN, (1L << 0), &key_press_unified, data);
	mlx_hook(data->win.win, ON_DESTROY, (1L << 17), &close_window, data);
	mlx_mouse_hook(data->win.win, &mouse_press_event, \
		&data->game.player.angle);
	mlx_loop_hook(data->mlx, &main_loop, data);
}

/*
git clone https://github.com/kamitsui/cub3D_parse_test
 valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes
  ./cub3D maps/success/test.cub 

  git remote -v
  git remote set-url origin 
  https://github.com/tototec1234/cub3d_mesasaki_torinoue.git
  git remote -v
	git push origin main
*/