/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_types.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: torinoue <torinoue@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 14:53:57 by torinoue          #+#    #+#             */
/*   Updated: 2025/11/07 00:20:07 by torinoue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_TYPES_H
# define CUB3D_TYPES_H

# include <stdbool.h>

typedef enum e_textures
{
	TEX_NORTH,
	TEX_SOUTH,
	TEX_WEST,
	TEX_EAST,
	TEX_COLLISION,
	TEXTURES,
}			t_textures;

typedef enum e_setting_errors
{
	INVALID_SETTING_ORDER,
	INVALID_TEXTURE_SETTING,
	INVALID_COLOR_SETTING,
	INVALID_IDENTIFIER,
	INVALID_MAP_ELEM,
	INVALID_NUMBER_MAP_ELEM,
	INVALID_MAP_UNCLOSED,
	INVALID_COLOR_VALUE,
	INVALID_MAP_FORMAT,
	SETTING_ERRORS,
	PLAYER_NOT_FOUND
}			t_setting_errors;

typedef enum e_defined_errors
{
	FILE_ERROR,
	MLX_ERROR,
	XPM_FILE_ERROR,
	INTERNAL_ERROR,
	INVALID_WINDOW_SIZE,
	INVALID_FOV_WIDTH,
	DEFINED_ERRORS
}			t_defined_errors;

typedef struct s_image
{
	char		*path;
	void		*image;
	void		*ptr;
	int			width;
	int			height;
	int			bits_per_pixel;
	int			bytes_per_pixel;
	int			line_length;
	int			endian;
}				t_image;

typedef enum e_wall_side
{
	HORIZONTAL,
	VERTICAL
}			t_wall_side;

typedef struct s_win
{
	void		*win;
	int			width;
	int			height;
}				t_win;

typedef struct s_map_coord
{
	double		x;
	double		y;
}				t_map_coord;

typedef struct s_player
{
	double		angle;
	double		fov;
	double		fov_per_win_width;
	t_map_coord	pos;
	bool		is_colliding;
}				t_player;

typedef struct s_rgb
{
	int			r;
	int			g;
	int			b;
}				t_rgb;

typedef struct s_header_parse_counters
{
	int			no_count;
	int			so_count;
	int			we_count;
	int			ea_count;
	int			f_count;
	int			c_count;
	bool		is_map_start;
}				t_header_parse_counters;

typedef struct s_reachable_bounds
{
	int	min_x;
	int	max_x;
	int	min_y;
	int	max_y;
}		t_reachable_bounds;

typedef struct s_map_context
{
	char				**map;
	int					*map_x_size;
	int					map_y_size;
	int					map_x_size_max;
	int					manhattan_distance;
	t_reachable_bounds	reachable_bounds;
	double				max_coord_x;
	double				max_coord_y;
	int					tile_size;
	double				tile_size_inv;
	double				tan_threshold_min;
	double				tan_threshold_max;
}						t_map_context;

typedef struct s_game_state
{
	t_player	player;
	int			ceiling_color;
}				t_game_state;

typedef struct s_data
{
	void			*mlx;
	t_win			win;
	t_image			screen;
	const char		*exec_name;
	char			*scene_filepath;
	t_map_context	map_ctx;
	t_image			tex[TEXTURES];
	t_game_state	game;
	int				original_ceiling_color;
	int				collision_ceiling_color;
	int				floor_color;
}					t_data;

typedef enum e_direction
{
	UP,
	DOWN,
	RIGHT,
	LEFT
}				t_direction;

typedef struct s_raycast
{
	double		angle;
	double		distance;
	double		tan_val;
	double		tan_inv;
	t_map_coord	intersection;
	t_direction	h_direction;
	t_direction	v_direction;
	int			wall_height;
	int			wall_top;
	int			wall_surplus;
	int			wall_bottom;
	t_wall_side	hit_side;
	double		tex_y_step;
	double		tex_y_pos;
}				t_raycast;

// Type definitions for graphics texture rendering
typedef struct s_render_params
{
	int	strip_id;
	int	x_coord;
}				t_render_params;

// Type definitions for key event handling
typedef struct s_data	t_data;	// Forward declaration

// Minimal context for key event handlers (encapsulation)
// Note: data field removed - exit handling uses dedicated hook
typedef struct s_key_event_context
{
	t_player			*player;
	const t_map_context	*map_ctx;
}						t_key_event_context;

// Callback function type for key handlers
typedef void			(*t_key_handler)(t_key_event_context *key_ctx);

// Key mapping structure
typedef struct s_key_mapping
{
	int				key_code;
	t_key_handler	handler;
	const char		*description;
}					t_key_mapping;

#endif
