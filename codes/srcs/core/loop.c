/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 13:16:59 by samatsum          #+#    #+#             */
/*   Updated: 2026/06/08 21:46:14 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/core.h"
#include <stddef.h>
#include <sys/time.h>

/* ************************************************************************** */
#define TARGET_FPS	30.0

int
	main_loop(t_game* game);
static long long
	get_current_time_ms(void);

/* ************************************************************************** */
// 毎フレーム実行されるメインループ。FPS制限とフレームレート非依存の移動・更新を行う
int
	main_loop(t_game* game)
{
	int			update;
	long long	now;
	double		delta_time;
	double		time_mult;
	double		speed_mult;

	now = get_current_time_ms();
	if (game->last_time == 0) {
		game->last_time = now;
	}
	delta_time = (double)(now - game->last_time) / 1000.0;
	
	/* FPSを30に制限する（前回フレームからの経過時間が1/30秒未満なら処理をスキップ） */
	if (delta_time < (1.0 / TARGET_FPS)) {
		return (0);
	}
	game->last_time = now;

	/* 基準となる60FPS時の移動量を1.0とし、経過時間からタイムマルチプライヤを計算 */
	time_mult = delta_time / (1.0 / 60.0);
	if (time_mult > 3.0) {
		time_mult = 3.0;
	}

	/* WEP_HANDS（3番）装備時は移動速度を1.3倍にする */
	speed_mult = 1.0;
	if (game->current_weapon == WEP_HANDS) {
		speed_mult = 1.3;
	}

	update = 0;
	if (game->move.x || game->move.y) {
		update = move_camera(&game->camera, &game->config, (game->move.x) ? 0 : 1, time_mult * speed_mult);
	}
	if (game->x_move.x || game->x_move.y) {
		update = move_perp_camera(&game->camera, &game->config, (game->x_move.x) ? 0 : 1, time_mult * speed_mult);
	}
	if (game->rotate.x || game->rotate.y) {
		update = rotate_camera(&game->camera, &game->config, (game->rotate.x) ? 0 : 1, time_mult); /* 回転は倍率の影響を受けない */
	}
	if (game->options != game->last_options) {
		update = 1;
		game->last_options = game->options;
	}
	if (update) {
		check_quest(game);
	}
	
	update_enemies(game, delta_time);

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
