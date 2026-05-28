/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* texture.c                                          :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2019/11/06 15:30:14 by samatsum          #+#    #+#             */
/* Updated: 2026/05/28 00:00:00 by samatsum         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "types.h"
#include "engine/texture/engine_texture.h"

static int
	load_tex(t_window *window, t_tex *tex, char *path)
{
	if (path)
	{
		tex->path = path;
		if ((tex->tex = mlx_xpm_file_to_image(window->ptr,
			path, &tex->width, &tex->height)))
			tex->ptr = mlx_get_data_addr(tex->tex,
				&tex->bpp, &tex->size_line, &tex->endian);
		else
			return (0);
	}
	return (1);
}

void
	clear_textures(t_window *window, t_tex *tex)
{
	int	i;

	i = 0;
	while (i < TEXTURES)
	{
		if (tex[i].tex)
			mlx_destroy_image(window->ptr, tex[i].tex);
		tex[i].tex = NULL;
		tex[i].ptr = NULL;
		i++;
	}
}

static int
	line_is_empty(t_tex *tex, int line)
{
	int		i;
	t_pos	pos;

	pos.y = line;
	i = 0;
	while (i < tex->width)
	{
		pos.x = i;
		if (get_tex_color(tex, &pos) != 0x0)
			return (0);
		i++;
	}
	return (1);
}

static int
	column_is_empty(t_tex *tex, int column)
{
	int		i;
	t_pos	pos;

	pos.x = column;
	i = 0;
	while (i < tex->height)
	{
		pos.y = i;
		if (get_tex_color(tex, &pos) != 0x0)
			return (0);
		i++;
	}
	return (1);
}

int
	load_textures(t_window *window, t_tex *tex, t_config *config)
{
	int	i;
	int	j;

	i = 0;
	while (i < TEXTURES)
	{
		if (!load_tex(window, &tex[i], config->tex_path[i]))
			return (0);
		j = 0;
		while (j < tex[i].height && column_is_empty(&tex[i], j))
			j++;
		tex[i].start.x = j;
		while (j < tex[i].height && !column_is_empty(&tex[i], j))
			j++;
		tex[i].end.x = j;
		j = 0;
		while (j < tex[i].height && line_is_empty(&tex[i], j))
			j++;
		tex[i].start.y = j;
		while (j < tex[i].height && !line_is_empty(&tex[i], j))
			j++;
		tex[i].end.y = j;
		i++;
	}
	return (1);
}