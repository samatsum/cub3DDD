/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 19:31:08 by samatsum          #+#    #+#             */
/*   Updated: 2026/06/07 06:59:09 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "core/core.h"
#include "engine/input/input.h"
#include "engine/input/keymap.h"
#include "engine/render/render.h"
#include "ui/ui.h"

/* ************************************************************************** */
#define KEY_NUM_1		49
#define KEY_NUM_2		50
#define KEY_NUM_3		51
#define KEY_SPACE		32
#define SHOOT_COOLDOWN	10

int
	expose_hook(t_game* game);
int
	exit_hook(t_game* game);
int
	key_press(int keycode, t_game* game);
int
	key_release(int keycode, t_game* game);

/* ************************************************************************** */
// ウィンドウ再描画イベント時の処理
int
	expose_hook(t_game* game)
{
	render_frame(game);
	return (0);
}

/* ************************************************************************** */
// ウィンドウの×ボタン等が押された際の終了処理
int
	exit_hook(t_game* game)
{
	return (exit_game(game, EXIT_SUCCESS));
}

/* ************************************************************************** */
// キーが押された際の移動や武器切り替え、射撃フラグの更新
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
	if (keycode == '1' || keycode == KEY_NUM_1) {
		game->current_weapon = WEP_PISTOL;
	}
	if (keycode == '2' || keycode == KEY_NUM_2) {
		game->current_weapon = WEP_FLASHLIGHT;
	}
	if (keycode == '3' || keycode == KEY_NUM_3) {
		game->current_weapon = WEP_HANDS;
	}
	if (keycode == KEY_SPACE && game->current_weapon == WEP_PISTOL) {
		if (game->is_shooting == 0) {
			game->is_shooting = SHOOT_COOLDOWN;
		}
	}
	return (0);
}

/* ************************************************************************** */
// キーが離された際の移動フラグ解除やUIオプションの切り替え
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
