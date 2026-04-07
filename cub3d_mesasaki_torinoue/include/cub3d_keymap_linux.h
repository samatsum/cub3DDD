/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_keymap_linux.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: torinoue <torinoue@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 14:53:57 by torinoue          #+#    #+#             */
/*   Updated: 2025/11/03 21:10:35 by torinoue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_KEYMAP_LINUX_H
# define CUB3D_KEYMAP_LINUX_H

# ifndef __APPLE__

// key code (Linux)
#  define MOUSE_WHEEL_UP			4
#  define MOUSE_WHEEL_DOWN		5
#  define KEY_A					97
#  define KEY_D					100
#  define KEY_S					115
#  define KEY_W					119
#  define KEY_ESC				65307
#  define KEY_LEFT				65361
#  define KEY_RIGHT				65363
// # define KEY_F6					65475

#  define ON_KEYDOWN				2
#  define ON_DESTROY				17

# endif

#endif
