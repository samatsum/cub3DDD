/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 19:31:08 by samatsum          #+#    #+#             */
/*   Updated: 2026/06/06 15:24:05 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h> /* system() 関数を使うため */
#include "core/core.h"  
#include "engine/input/input.h" 
#include "engine/input/keymap.h" 
#include "engine/render/render.h" 
#include "ui/ui.h"                

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
// キーが押された際に、ゲーム内のフラグをオンにする
int
	key_press(int keycode, t_game* game)
{
	int	unused; /* 警告回避用のダミー変数 */

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

	/* 武器の切り替え */
	if (keycode == '1' || keycode == 49) {
		game->current_weapon = WEP_PISTOL;
	}
	if (keycode == '2' || keycode == 50) {
		game->current_weapon = WEP_FLASHLIGHT;
	}

	/* スペースキーで射撃アクションと効果音再生 */
	if (keycode == 32 && game->current_weapon == WEP_PISTOL) {
		if (game->is_shooting == 0) {
			game->is_shooting = 1;
			
			/* Linux環境で音を鳴らすコマンド (WSL環境だからか、鳴らないよ～)*/
			/* 警告回避: 戻り値を unused に入れ、それを void キャストで明示的に捨てる */
			/* ログ非表示: > /dev/null 2>&1 を付け、ターミナルが汚れるのを防ぐ */
			unused = system("aplay sounds/shoot.mp3 > /dev/null 2>&1 &");
			(void)unused;
		}
	}
	return (0);
}

/* ************************************************************************** */
// キーが離された際に、フラグのオフやオプションの切り替えを行う
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

	/* スペースキーを離したら射撃状態を解除 */
	if (keycode == 32 && game->current_weapon == WEP_PISTOL) {
		game->is_shooting = 0;
	}
	return (0);
}
