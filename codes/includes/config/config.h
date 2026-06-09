/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 12:51:26 by samatsum          #+#    #+#             */
/*   Updated: 2026/06/08 22:19:56 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_H
# define CONFIG_H

# include "utils/utils.h"
# include <math.h> /* floor関数を使用するマクロ(FINT)のため追加 */

/* ************************************************************************** */
# define DIRECTIONS				"NSEW"
# define VALID_MAP_CHARACTERS	" 01234EWNSM"

# define MAX_WIDTH				1920
# define MAX_HEIGHT				1080
# define MIN_WIDTH				848
# define MIN_HEIGHT				480

# define FOV_SCALE				2.5

# define FINT(x)				((int)floor(x))
# define CHECK_TOP(p)			(FINT(p.x) >= 0 && FINT(p.y) >= 0)
# define CHECK_BOT(p, c)		(FINT(p.x) < (c).map.columns && FINT(p.y) < (c).map.rows)
# define IN_MAP(p, c)			(CHECK_TOP(p) && CHECK_BOT(p, c))
# define MAP(p, c)				(c).map.data[(FINT(p.y) * (c).map.columns) + FINT(p.x)]
# define MAP_XY(x, y, c)		(c).map.data[(FINT(y) * (c).map.columns) + FINT(x)]

# define C_R					0
# define C_NO					1
# define C_SO					2
# define C_WE					3
# define C_EA					4
# define C_OI					5
# define C_OP					6
# define C_OC					7
# define C_FT					8
# define C_ST					9
# define C_F					10
# define C_C					11

# define C_MAP					12
# define C_LAST					13

# define TEXTURES				9
# define TEX_NORTH				0
# define TEX_SOUTH				1
# define TEX_WEST				2
# define TEX_EAST				3
# define TEX_SKY				4
# define TEX_FLOOR				5
# define TEX_SPRITE				6
# define TEX_SPRITE_UP			7
# define TEX_SPRITE_C			8

# define BEST_RATIO				1.7777777778

/* ************************************************************************** */
typedef struct s_map
{
	int*	data;
	int		columns;
	int		rows;
}				t_map;

typedef struct s_config
{
	char*			tex_path[TEXTURES];
	double			rotate_speed;
	double			move_speed;
	double			fov;
	unsigned int	requested_width;
	unsigned int	requested_height;
	unsigned int	colors[TEXTURES];
	int				set[C_LAST];
	t_map			map;
}				t_config;

/* ************************************************************************** */
void
	init_config(t_config* config);
int
	clear_config(t_config* config);
int
	parse_dimensions(t_config* config, char const* line);
int
	parse_texture(t_config* config, int key, char const* line);
int
	parse_color(t_config* config, int key, char const* line);
int
	parse_config(t_config* config, char const* conf_path);
int
	count_check_columns(char const* line);
int
	check_top_bottom_borders(t_str* map_buffer);
int
	check_left_right_borders(t_str* map_buffer);
int
	check_valid(t_config* config, t_str* map_buffer);
int
	parse_map(t_config* config, t_str* map_buffer);

#endif
