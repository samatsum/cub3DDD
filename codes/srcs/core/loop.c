/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 13:16:59 by samatsum          #+#    #+#             */
/*   Updated: 2026/06/03 08:52:45 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "engine/raycast/raycast.h" /* 名前を変更している場合 */

/* ************************************************************************** */
int
	main_loop(t_game* game);

/* ************************************************************************** */
// 毎フレーム実行されるメインループ。入力に応じたカメラの移動と画面描画を行う
int
	main_loop(t_game* game)
{
	int	update;

	update = 0;
	if (game->move.x || game->move.y) {
		update = move_camera(&game->camera, &game->config, (game->move.x) ? 0 : 1);
	}
	if (game->x_move.x || game->x_move.y) {
		update = move_perp_camera(&game->camera, &game->config, (game->x_move.x) ? 0 : 1);
	}
	if (game->rotate.x || game->rotate.y) {
		update = rotate_camera(&game->camera, game->cos, game->sin, (game->rotate.x) ? 0 : 1);
	}
	if (game->options != game->last_options) {
		update = 1;
		game->last_options = game->options;
	}
	if (update) {
		check_quest(game);
	}
	if (update || (FLAG_UI & game->options)) {
		render_frame(game);
	}
	return (1);
}
