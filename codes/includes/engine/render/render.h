/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 13:16:59 by samatsum          #+#    #+#             */
/*   Updated: 2026/06/03 14:31:25 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDER_H
# define RENDER_H

# include "utils/utils.h"
# include "engine/texture/texture.h" /* t_tex を実体やメンバとして使用するため */
# include "engine/raycast/raycast.h" /* t_camera, t_raysult のため */

/* ************************************************************************** */
typedef struct s_image
{
	void*		img;
	void*		ptr;
	int			bpp;
	int			size_line;
	int			endian;
}				t_image;

typedef struct s_window
{
	void*		ptr;
	void*		win;
	t_image		screen;
	t_pos		size;
	t_pos		half;
	double		ratio;
}				t_window;

typedef struct s_sprite_draw
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

typedef struct s_sprite
{
	t_pos				pos;
	double				distance;
	t_tex*				tex;
	struct s_sprite*	next;
	struct s_sprite*	sorted;
}				t_sprite;

struct s_config;
struct s_game;

typedef struct s_render
{
	t_window*			w;
	struct s_config*	config;
	t_camera*			camera;
	t_tex*				tex;
	double*				depth;
	double*				sf_dist;
	int					options;
}				t_render;

/* ************************************************************************** */
int
	init_window(t_window* window, struct s_config* config);
int
	clear_window(t_window* window);
void
	update_window(t_window* w, int options, int collected, int to_collect);
void
	render_frame(struct s_game* game);
void
	update_screen(struct s_game* game);
int
	init_image(t_window* window, t_image* img);
void
	destroy_image(t_window* window, t_image* img);
void
	draw_pixel(t_window* w, t_pos* pos, int color);
int
	draw_vertical_line(t_window* window, t_pos* start, int length, int color);
int
	draw_rectangle(t_window* window, t_pos* p1, t_pos* p2, int color);
int
	draw_string(t_window* window, t_pos* s_pos, char* str, int color);
void
	draw_wall(t_render* rnd, t_raysult* ray);
void
	draw_sky_floor(t_render* rnd, t_raysult* ray);
t_sprite*
	add_front_sprite(t_sprite** sprites, double distance, t_pos* pos, t_tex* tex);
t_sprite*
	add_sorted_sprite(t_sprite** sprites, t_sprite* sprite);
t_sprite*
	sort_sprites(t_camera* camera, t_sprite* sprites);
void
	delete_sprite(t_sprite** sprites, t_pos* pos);
void
	draw_sprites(t_render* rnd, t_sprite* sprites);
void
	clear_sprites(t_sprite** sprites);

#endif
