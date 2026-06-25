/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 13:16:59 by samatsum          #+#    #+#             */
/*   Updated: 2026/06/15 12:15:07 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXTURE_H
# define TEXTURE_H

# include "utils/utils.h"

/* ************************************************************************** */
typedef struct s_tex
{
	char*		path;
	void*		tex;
	void*		ptr;
	t_pos		start;
	t_pos		end;
	int			width;
	int			height;
	int			bpp;
	int			size_line;
	int			endian;
}				t_tex;

/* 前方宣言（ポインタのみ使用するためインクルード不要） */
struct s_window;
struct s_config;

/* ************************************************************************** */
int
	load_textures(struct s_window* window, t_tex* tex, struct s_config* config);
void
	clear_textures(struct s_window* window, t_tex* tex);
int
	load_tex_image(struct s_window* window, t_tex* tex);
int
	shade_color(int color, double divide);
int
	distance_shade(int options, int color, double distance, double light);
int
	get_tex_color(t_tex* tex, t_pos* pos);
double
	flashlight_divide(double divide, double distance, double light);

#endif
