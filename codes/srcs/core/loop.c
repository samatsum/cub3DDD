/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 13:16:59 by samatsum          #+#    #+#             */
/*   Updated: 2026/06/04 02:35:37 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/core.h"
#include <stddef.h> /* NULL 用 */

/* ************************************************************************** */
int
	main_loop(t_game* game);
static long long
	get_current_time_ms(void);

/* ************************************************************************** */
// 毎フレーム実行されるメインループ。経過時間を計測し、フレームレート非依存の移動を行う
int
	main_loop(t_game* game)
{
	int			update;
	long long	now;
	double		delta_time;
	double		time_mult;

	/* 修正: 経過時間（デルタタイム）の計算 */
	now = get_current_time_ms();
	if (game->last_time == 0) {
		game->last_time = now;
	}
	delta_time = (double)(now - game->last_time) / 1000.0; /* 秒に変換 */
	game->last_time = now;

	/* 60FPS (1秒間に60フレーム) を基準(1.0)とした補正倍率を計算 */
	time_mult = delta_time / (1.0 / 60.0);
	/* 異常なラグ発生時のワープを防ぐための安全装置 (最大でも3フレーム分) */
	if (time_mult > 3.0) {
		time_mult = 3.0;
	}

	update = 0;
	if (game->move.x || game->move.y) {
		update = move_camera(&game->camera, &game->config, (game->move.x) ? 0 : 1, time_mult);
	}
	if (game->x_move.x || game->x_move.y) {
		update = move_perp_camera(&game->camera, &game->config, (game->x_move.x) ? 0 : 1, time_mult);
	}
	if (game->rotate.x || game->rotate.y) {
		update = rotate_camera(&game->camera, &game->config, (game->rotate.x) ? 0 : 1, time_mult);
	}
	if (game->options != game->last_options) {
		update = 1;
		game->last_options = game->options;
	}
	if (update) {
		check_quest(game);
	}
	/*update判定に関わらず毎フレーム描画を維持 */
	render_frame(game);
	return (1);
}

/* ************************************************************************** */
// 現在のシステム時刻をミリ秒単位で取得する
static long long
	get_current_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((long long)tv.tv_sec * 1000 + (long long)tv.tv_usec / 1000);
}
