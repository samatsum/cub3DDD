/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 07:32:33 by samatsum          #+#    #+#             */
/*   Updated: 2026/06/03 10:43:48 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CORE_H
# define CORE_H

/* ************************************************************************** */
# define FLAG_UI			0x00000010
# define FLAG_SHADOWS		0x00000100
# define FLAG_CROSSHAIR		0x00001000
# define FLAG_SAVE			0x00000001

/* ************************************************************************** */
struct s_game;

/* ************************************************************************** */
int
	main_loop(struct s_game* game);
void
	init_game(struct s_game* game);
int
	finish_init(struct s_game* game);
int
	exit_game(struct s_game* game, int code);
int
	exit_error(struct s_game* game, const char* str);
int
	screenshot(struct s_game* game);
void
	check_quest(struct s_game* game);
void
	count_items(struct s_game* game);

#endif
