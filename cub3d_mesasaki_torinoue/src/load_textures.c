/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_textures.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toruinoue <toruinoue@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 21:03:45 by torinoue          #+#    #+#             */
/*   Updated: 2025/11/04 01:57:54 by toruinoue        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static void	load_texture_from_xpm(t_data *data, t_image *tex);

void	load_textures(t_data *data)
{
	int	i;

	i = 0;
	while (i < TEXTURES)
	{
		if (data->tex[i].path)
			load_texture_from_xpm(data, &data->tex[i]);
		i++;
	}
}

static void	load_texture_from_xpm(t_data *data, t_image *tex)
{
	tex->image = mlx_xpm_file_to_image(data->mlx, tex->path, &tex->width,
			&tex->height);
	if (!tex->image)
		display_error(XPM_FILE_ERROR, data);
	tex->ptr = mlx_get_data_addr(tex->image, &tex->bits_per_pixel,
			&tex->line_length, &tex->endian);
	if (!tex->ptr)
		display_error(MLX_ERROR, data);
	tex->bytes_per_pixel = tex->bits_per_pixel / 8;
}
