/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   item.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 07:32:33 by samatsum          #+#    #+#             */
/*   Updated: 2026/06/03 08:52:19 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/* ************************************************************************** */
void
	check_quest(t_game* game);
void
	count_items(t_game* game);

/* ************************************************************************** */
// プレイヤーの現在位置にあるアイテムを取得したか判定し、状態を更新する
void
	check_quest(t_game* game)
{
	if (MAP(game->camera.pos, game->config) == '4') {
		MAP(game->camera.pos, game->config) = 'A';
		game->collected++;
		delete_sprite(&game->sprites, &game->camera.pos);
	}
}

/* ************************************************************************** */
// マップ上に存在する収集アイテムの総数を数える
void
	count_items(t_game* game)
{
	int	i;
	int	j;

	game->to_collect = 0;
	i = 0;
	while (i < game->config.map.rows) {
		j = 0;
		while (j < game->config.map.columns) {
			if (MAP_XY(j, i, game->config) == '4') {
				game->to_collect++;
			}
			j++;
		}
		i++;
	}
}
