/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/31 12:51:35 by samatsum          #+#    #+#             */
/*   Updated: 2026/05/27 20:24:48 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int
	screenshot(t_game *game)
{
	update_screen(game);
	update_ui(game);
	update_window(game);
	if (!save_bmp(game))
		exit_error(game, "Error:\nfailed to save screenshot.");
	return (exit_game(game, EXIT_SUCCESS));
}


void
	check_quest(t_game *game)
{
	if (MAP(game->camera.pos, game->config) == '4')
	{
		MAP(game->camera.pos, game->config) = '0';
		game->collected++;
		delete_sprite(&game->sprites, &game->camera.pos);
	}
}

void
	count_items(t_game *game)
{
	int	i;
	int	j;

	game->to_collect = 0;
	i = 0;
	while (i < game->config.rows)
	{
		j = 0;
		while (j < game->config.columns)
		{
			if (MAP_XY(j, i, game->config) == '4')
				game->to_collect++;
			j++;
		}
		i++;
	}
}
