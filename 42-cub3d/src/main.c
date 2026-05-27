/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/29 12:45:06 by samatsum          #+#    #+#             */
/*   Updated: 2026/05/27 15:02:20 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "include/cub3d.h"

int	exit_error(t_game *game, char const *str)
{
	int	unused;

	if (str)
		unused = write(STDOUT_FILENO, str, ft_strlen(str));
	(void)unused;
	exit_game(game, EXIT_FAILURE);
	return (EXIT_FAILURE);
}

int	main(int argc, char **argv)
{
	t_game	game;
	int		save_opt;

	save_opt = (argc >= 2 && !ft_strcmp(argv[1], "-save"));
	if (argc < (2 + save_opt))
		return (exit_error(&game, "Error:\nno map specified.\n"));
	init_game(&game, save_opt);
	if (!parse_config(&game.config, argv[1 + save_opt]))
		return (exit_error(&game, "Error:\ninvalid map.\n"));
	if (!finish_init(&game))
		return (EXIT_FAILURE);
	if (game.options & FLAG_SAVE)
		return (screenshot(&game));
	mlx_hook(game.window.win, X_EVENT_KEY_PRESS, MASK_KEY_PRESS, &key_press, &game);
	mlx_hook(game.window.win, X_EVENT_KEY_RELEASE, MASK_KEY_RELEASE, &key_release, &game);
	mlx_hook(game.window.win, X_EVENT_EXIT, MASK_CLOSE, &exit_hook, &game);
	mlx_hook(game.window.win, EVENT_EXPOSE, MASK_EXPOSE, &expose_hook, &game);
	mlx_loop_hook(game.window.ptr, &main_loop, &game);
	mlx_loop(game.window.ptr);
	return (EXIT_SUCCESS);
}
