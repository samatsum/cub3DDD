/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 07:32:33 by samatsum          #+#    #+#             */
/*   Updated: 2026/06/08 17:20:49 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CORE_H
# define CORE_H

/* ************************************************************************** */
# include <sys/time.h> /* gettimeofday 等による時間計測のため */
# include "config/config.h"
# include "engine/render/render.h"
# include "engine/raycast/raycast.h"
# include "enemy/enemy.h"

/* ************************************************************************** */
# define FLAG_UI			0x00000010
# define FLAG_SHADOWS		0x00000100
# define FLAG_CROSSHAIR		0x00001000
# define FLAG_SAVE			0x00000001

# define WEP_PISTOL			0
# define WEP_FLASHLIGHT		1
# define WEP_HANDS			2

/* ************************************************************************** */
typedef struct s_game
{
	t_config		config;
	t_window		window;
	t_camera		camera;
	t_sprite*		sprites;
	t_enemy*		enemies;  /* ← 敵を管理する専用リストを追加 */
	t_tex			tex[TEXTURES];
	t_tex			weapon_tex[6];  /* 0:待機, 1:発砲, 2:反動, 3:懐中電灯, 4:左手, 5:右手 */
	t_tex			enemy_tex[8]; /* ← 追加: 敵の8方向テクスチャ配列 */
	int				current_weapon; /* 現在の武器 (WEP_PISTOL 等) */
	int				is_shooting;    /* 射撃アニメーションタイマー (残りフレーム数) */
	t_pos			move;
	t_pos			x_move;
	t_pos			rotate;
	int				options;
	int				last_options;
	int				to_collect;
	int				collected;
	long long		last_time;
	double			camera_x[1920];
	double			depth[1920];
	double			sf_dist[1080];
	double			cos[2];
	double			sin[2];
}				t_game;

/* ************************************************************************** */
int
	main_loop(t_game* game);
void
	init_game(t_game* game);
int
	finish_init(t_game* game);
int
	exit_game(t_game* game, int code);
int
	exit_error(t_game* game, const char* str);
int
	screenshot(t_game* game);
void
	check_quest(t_game* game);
void
	count_items(t_game* game);
void
	shoot_target(t_game* game);

#endif
