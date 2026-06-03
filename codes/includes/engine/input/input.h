/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 19:14:41 by samatsum          #+#    #+#             */
/*   Updated: 2026/06/03 14:35:18 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_H
# define INPUT_H
# include "core/core.h"

/* ************************************************************************** */
int
	exit_hook(t_game* game);
int
	expose_hook(t_game* game);
int
	key_press(int keycode, t_game* game);
int
	key_release(int keycode, t_game* game);

#endif