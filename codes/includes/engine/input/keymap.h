/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keymap.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 14:24:58 by samatsum          #+#    #+#             */
/*   Updated: 2026/06/10 08:27:32 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KEYMAP_H
# define KEYMAP_H

/* ************************************************************************** */
// X11 イベント種別の番号
# define X_EVENT_KEY_PRESS		2
# define X_EVENT_KEY_RELEASE	3
# define X_EVENT_MOUSE_PRESS	4
# define X_EVENT_MOUSE_RELEASE	5
# define X_EVENT_MOUSE_MOVE		6
# define X_EVENT_EXIT			17

// マウスボタン・ホイールの番号
# define LEFT_CLICK				1
# define RIGHT_CLICK			2
# define MIDDLE_CLICK			3
# define SCROLL_UP				4
# define SCROLL_DOWN			5

// 移動キー（WASDのみ）。上下矢印は移動に割り当てない
# define KEY_W					119
# define KEY_A					97
# define KEY_S					115
# define KEY_D					100

// 回転キー（Q/E と 左右矢印）
# define KEY_Q					113
# define KEY_E					101
# define KEY_LEFT				65361
# define KEY_RIGHT				65363

// 上下矢印（現在は移動から除外。将来の再割り当て用に残置）
# define KEY_FORWARD			65362
# define KEY_BACKWARD			65364

// 機能キー（終了・UI・影・照準）
# define KEY_ESC				65307
# define KEY_I					105
# define KEY_L					108
# define KEY_O					111

// ウィンドウイベントとイベントマスク
# define EVENT_CLOSE			33
# define EVENT_EXPOSE			12
# define MASK_KEY_PRESS			(1L << 0)
# define MASK_KEY_RELEASE		(1L << 1)
# define MASK_CLOSE				(1L << 17)
# define MASK_EXPOSE			(1L << 15)
# define EVENT_KEY_PRESS		2
# define EVENT_KEY_RELEASE		3

/* ************************************************************************** */

#endif
