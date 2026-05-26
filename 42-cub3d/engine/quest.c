/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quest.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/10 13:18:01 by samatsum          #+#    #+#             */
/*   Updated: 2026/05/26 22:37:16 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/engine.h"

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
