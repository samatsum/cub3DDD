/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 19:31:08 by samatsum          #+#    #+#             */
/*   Updated: 2026/06/03 14:33:39 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/core.h"  

#include "engine/input/input.h" /* フック関数のプロトタイプを使うため */
#include "engine/input/keymap.h" /* キーコード（KEY_W等*/
#include "engine/render/render.h" /* render_frame関数を呼び出すため */
#include "ui/ui.h"                /* shortcuts.c の関数などを呼び出している場合 */

/* ************************************************************************** */
int
	expose_hook(t_game* game);
int
	exit_hook(t_game* game);
int
	key_press(int keycode, t_game* game);
int
	key_release(int keycode, t_game* game);

/* ************************************************************************** */
// ウィンドウが再描画されるべきタイミングで画面を更新する
int
	expose_hook(t_game* game)
{
	render_frame(game);
	return (0);
}

/* ************************************************************************** */
// ウィンドウの×ボタンなどが押された際にゲームを終了させる
int
	exit_hook(t_game* game)
{
	return (exit_game(game, EXIT_SUCCESS));
}

/* ************************************************************************** */
// キーが押された際に、ゲーム内の移動フラグや回転フラグをオンにする
int
	key_press(int keycode, t_game* game)
{
	if (keycode == KEY_W || keycode == KEY_FORWARD) {
		game->move.x = 1;
	} else if (keycode == KEY_S || keycode == KEY_BACKWARD) {
		game->move.y = 1;
	}
	if (keycode == KEY_A) {
		game->x_move.x = 1;
	} else if (keycode == KEY_D) {
		game->x_move.y = 1;
	}
	if (keycode == KEY_Q || keycode == KEY_LEFT) {
		game->rotate.x = 1;
	} else if (keycode == KEY_E || keycode == KEY_RIGHT) {
		game->rotate.y = 1;
	}
	return (0);
}

/* ************************************************************************** */
// キーが離された際に、移動フラグのオフやオプションの切り替えを行う
int
	key_release(int keycode, t_game* game)
{
	if (keycode == KEY_W || keycode == KEY_FORWARD) {
		game->move.x = 0;
	} else if (keycode == KEY_S || keycode == KEY_BACKWARD) {
		game->move.y = 0;
	} else if (keycode == KEY_A) {
		game->x_move.x = 0;
	} else if (keycode == KEY_D) {
		game->x_move.y = 0;
	} else if (keycode == KEY_Q || keycode == KEY_LEFT) {
		game->rotate.x = 0;
	} else if (keycode == KEY_E || keycode == KEY_RIGHT) {
		game->rotate.y = 0;
	} else if (keycode == KEY_ESC) {
		return (exit_game(game, EXIT_SUCCESS));
	} else if (keycode == KEY_I) {
		game->options = game->options ^ FLAG_UI;
	} else if (keycode == KEY_L) {
		game->options = game->options ^ FLAG_SHADOWS;
	} else if (keycode == KEY_O) {
		game->options = game->options ^ FLAG_CROSSHAIR;
	}
	return (0);
}
