/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_macros.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: torinoue <torinoue@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 14:53:57 by torinoue          #+#    #+#             */
/*   Updated: 2025/11/13 18:34:42 by torinoue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_MACROS_H
# define CUB3D_MACROS_H

# include <math.h>

// constant
# define PI						M_PI

# define WIN_WIDTH				2000
# define WIN_HEIGHT				1000

// config
# define DEFAULT_TILE_SIZE		64
# define DEFAULT_TEXTURE_SIZE	64
# define DEFAULT_CEILING_COLOR	0xB00000
# define ON_WALL_CEILING_BRIGHTNESS 0.5
# define DEFAULT_FLOOR_COLOR 	0x614224
// # define DEFAULT_FOV			0.00000000000000000	// test 
// # define DEFAULT_FOV			0.08726646259971647	// 5 degree = PI / 36
// # define DEFAULT_FOV			0.26179938779914941	// 15 degree = PI / 12
// # define DEFAULT_FOV			0.26179938779914941	// 15 degree = PI / 12
// # define DEFAULT_FOV			0.52359877559829882	// 30 degree = PI / 6
// # define DEFAULT_FOV			0.78539816339744828	// 45 degree = PI / 4
// # define DEFAULT_FOV			1.04719755119659763	// 60 degree = PI / 3
// # define DEFAULT_FOV			1.30899693899574721	// 75 degree = PI * 5 / 12
# define DEFAULT_FOV			1.57079632679489656	// 90 degree = PI / 2
// # define DEFAULT_FOV			1.83259571459404613	// 105 degree = PI * 7 / 12
// # define DEFAULT_FOV			2.09439510239319526	// 120 degree = PI * 2 / 3
// # define DEFAULT_FOV			2.35619449019234484	// 135 degree = PI * 3 / 4
// # define DEFAULT_FOV			2.61799387799149441	// 150 degree = PI * 5 / 6
// # define DEFAULT_FOV			2.87979326579064354	// 165 degree = PI * 11 / 12
// # define DEFAULT_FOV			3.14159265358979312	// 180 degree = PI
// # define DEFAULT_FOV			3.40339204138894269	// 195 degree = PI * 13 / 12
// # define DEFAULT_FOV			3.66519142918809226	// 210 degree = PI * 7 / 6
// # define DEFAULT_FOV			3.92699081698724139	// 225 degree = PI * 5 / 4
// # define DEFAULT_FOV			4.18879020478639053	// 240 degree = PI * 4 / 3
// # define DEFAULT_FOV			4.45058959258554054	// 255 degree = PI * 17 / 12
// # define DEFAULT_FOV			4.71238898038468967	// 270 degree = PI * 3 / 2
// # define DEFAULT_FOV			4.97418836818383880	// 285 degree = PI * 19 / 12
// # define DEFAULT_FOV			5.23598775598298882	// 300 degree = PI * 5 / 3
// # define DEFAULT_FOV			5.49778714378213795	// 315 degree = PI * 7 / 4
// # define DEFAULT_FOV			5.75958653158128708	// 330 degree = PI * 11 / 6
// # define DEFAULT_FOV			6.02138591938043710	// 345 degree = PI * 23 / 12
// # define DEFAULT_FOV			6.28318530717958623	// 360 degree = 2 * PI
# define DEFAULT_MOVE_VALUE 	4
# define DEFAULT_ROTATE_VALUE 	0.05
// # define DEFAULT_ROTATE_VALUE 0.005

// raycasting
# define WALL_OFFSET			0.0001

// Output file descriptors
# define DEBUG_FD				1
# define ERROR_FD				2
# define INFO_FD				1

#endif
