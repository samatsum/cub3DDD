/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/29 12:44:11 by samatsum          #+#    #+#             */
/*   Updated: 2026/05/27 15:11:49 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "engine.h"
# include "config.h"
# include "utils.h"
# include "keymap.h"

# define LEFT_CLICK 	1
# define RIGHT_CLICK 	2
# define MIDDLE_CLICK	3
# define SCROLL_UP		4
# define SCROLL_DOWN	5


int				exit_error(t_game *game, char const *str);

void			init_game(t_game *game, int save_opt);

int				finish_init(t_game *game);

int				exit_game(t_game *game, int code);

int				screenshot(t_game *game);

int				exit_hook(t_game *game);

int				expose_hook(t_game *game);

int				key_press(int keycode, t_game *game);

int				key_release(int keycode, t_game *game);

int				main_loop(t_game *game);

int				save_bmp(t_game *game);

#endif
