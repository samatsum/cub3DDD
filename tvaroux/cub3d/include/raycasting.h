/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 15:10:29 by hyamamot          #+#    #+#             */
/*   Updated: 2026/02/20 15:10:33 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAYCASTING_H
# define RAYCASTING_H

# include "main.h"

# include <mlx.h>
# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <sys/time.h>

# define HEIGHT 480
# define WIDTH 680
# define SIZE 24
# define WHITE 0x00FFFFFF
# define MAX_HELD 6
# define KEY_ESC 65307
# define KEY_W 119
# define KEY_A 97
# define KEY_S 115
# define KEY_D 100
# define KEY_RIGHT 65363
# define KEY_LEFT 65361

typedef struct s_data
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	int		w;
	int		h;
}	t_data;

typedef struct s_vars
{
	void	*mlx;
	void	*win;
	t_data	img;
	int		hold_keys[MAX_HELD];
	double	pos_x;
	double	pos_y;
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
	double	time_ms;
	t_data	tex[4];
	char	**texture_path;
	int		color_code[2];
	int		**map;
}	t_vars;

typedef struct s_ray
{
	double	t;
	double	ray_dir_x;
	double	ray_dir_y;
	int		map_x;
	int		map_y;
	double	side_dist_x;
	double	side_dist_y;
	double	delta_dist_x;
	double	delta_dist_y;
	double	ray_l;
	int		step_x;
	int		step_y;
	int		hit;
	int		side;
	int		line_height;
	int		draw_start;
	int		draw_end;
	double	wall_x;
}	t_ray;

int				on_key_press(int keycode, t_vars *vars);
int				on_key_release(int keycode, t_vars *vars);
int				on_destroy(t_vars *vars);
void			my_mlx_pixel_put(t_data *data, int x, int y, int color);
double			get_time_ms(void);
void			clear_image(t_data *img);
int				get_move_input(t_vars *vars);
void			img_pixel_put(t_data *img, int x, int y, unsigned int color);
unsigned int	img_pixel_get(t_data *img, int x, int y);
void			render_texture(t_vars *vars, t_ray *ray, int x);
void			render_ceiling_and_floor(t_vars *vars, int x, int line_height);
int				render_fram(t_vars *vars);
void			ray_init(t_vars *vars, t_ray *ray, int x);
void			ray_dda(t_vars *vars, t_ray *ray);
void			raycasting(t_map *map);

#endif
