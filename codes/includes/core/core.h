/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 07:32:33 by samatsum          #+#    #+#             */
/*   Updated: 2026/06/03 14:31:50 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CORE_H
# define CORE_H

# include "config/config.h"
# include "engine/render/render.h"
# include "engine/raycast/raycast.h"

/* ************************************************************************** */
# define FLAG_UI			0x00000010
# define FLAG_SHADOWS		0x00000100
# define FLAG_CROSSHAIR		0x00001000
# define FLAG_SAVE			0x00000001

/* ************************************************************************** */
typedef struct s_game
{
	t_config	config;
	t_window	window;
	t_camera	camera;
	t_sprite*	sprites;
	t_tex		tex[TEXTURES];
	t_pos		move;
	t_pos		x_move;
	t_pos		rotate;
	int			options;
	int			last_options;
	int			to_collect;
	int			collected;
	double		camera_x[1920];
	double		depth[1920];
	double		sf_dist[1080];
	double		cos[2];
	double		sin[2];
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

#endif
