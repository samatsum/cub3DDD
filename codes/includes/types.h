#ifndef TYPES_H
# define TYPES_H

# include <math.h>
# include "../minilibx-linux/mlx.h"
# include "config/config.h"
/* 注: t_pos などの定義がある utils.h 等が必要な場合は適宜インクルードする */

# define X_EVENT_KEY_PRESS		2
# define X_EVENT_KEY_RELEASE	3
# define X_EVENT_MOUSE_PRESS	4
# define X_EVENT_MOUSE_RELEASE	5
# define X_EVENT_MOUSE_MOVE		6
# define X_EVENT_EXIT			17

# define FLAG_UI			0x00000010
# define FLAG_SHADOWS		0x00000100
# define FLAG_CROSSHAIR		0x00001000
# define FLAG_SAVE			0x00000001

# define BEST_RATIO			1.7777777778

typedef struct	s_raysult
{
	int			column;
	int			row;
	double		distance;
	int			direction;
	int			side;
	int			height;
	t_pos		ray_pos;
	t_pos		ray_dir;
	t_pos		map_pos;
	t_pos		side_dist;
	t_pos		delta_dist;
	t_pos		step;
	double		wall_x;
	t_pos		floor_wall;
	t_pos		c_floor;
}				t_raysult;

typedef struct	s_image
{
	void	*img;
	void	*ptr;
	int		bpp;
	int		size_line;
	int		endian;
}				t_image;

typedef struct	s_tex
{
	char	*path;
	void	*tex;
	void	*ptr;
	t_pos	start;
	t_pos	end;
	int		width;
	int		height;
	int		bpp;
	int		size_line;
	int		endian;
}				t_tex;

typedef struct	s_sprite_draw
{
	int			sprite_screen;
	t_pos		pos;
	t_pos		transform;
	t_pos		spr_s;
	t_pos		draw_x;
	t_pos		draw_y;
	t_pos		tex_pos;
	int			fact;
	int			draw_y_org;
}				t_sprite_draw;

typedef struct	s_sprite
{
	t_pos			pos;
	double			distance;
	t_tex			*tex;
	struct s_sprite	*next;
	struct s_sprite	*sorted;
}				t_sprite;

typedef	struct	s_window
{
	void		*ptr;
	void		*win;
	t_image		screen;
	t_pos		size;
	t_pos		half;
	double		ratio;
}				t_window;

typedef struct	s_camera
{
	t_pos	pos;
	t_pos	dir;
	t_pos	x_dir;
	t_pos	plane;
}				t_camera;

typedef struct	s_render
{
	t_window	*w;
	t_config	*config;
	t_camera	*camera;
	t_tex		*tex;
	double		*depth;
	double		*sf_dist;
	int			options;
}				t_render;

typedef	struct	s_game
{
	t_config	config;
	t_window	window;
	t_camera	camera;
	t_sprite	*sprites;
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

#endif
