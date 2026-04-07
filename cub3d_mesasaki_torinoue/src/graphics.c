/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toruinoue <toruinoue@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 19:15:06 by torinoue          #+#    #+#             */
/*   Updated: 2025/11/09 01:12:33 by toruinoue        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

/*
** Graphics abstraction layer for window-wide pixel operations
** This module provides high-level graphics functions that operate on the entire
** window, abstracting away low-level pixel manipulation details.
*/

/*
** Draws a 2D background by filling the entire window with ceiling and floor
** colors. This is a simple flat background rendering that divides the window
** horizontally into two sections: upper half (ceiling) and lower half (floor)
**
** @param image: Image buffer to draw on
** @param win: Window dimensions
** @param ceiling_color: Color for the upper half
** @param floor_color: Color for the lower half
*/
void	draw_background(t_image *image, const t_win *win, \
	const int ceiling_color, const int floor_color)
{
	int	i;
	int	j;

	i = 0;
	while (i < win->height)
	{
		j = 0;
		while (j < win->width)
		{
			if (i < win->height / 2)
				pixel_set(image, j, i, ceiling_color);
			else
				pixel_set(image, j, i, floor_color);
			j++;
		}
		i++;
	}
}

/*
** Renders textured pixels for a vertical strip of the window
** This function performs 3D texture mapping by calculating UV coordinates
** and applying texture pixels to the screen buffer
**
** @param data: Game data containing screen and tile information
** @param strip_id: Horizontal position of the vertical strip
** @param texture: Texture image to map onto the wall
** @param ray: Raycast data containing intersection and wall information
*/
void	render_textured_pixels(t_data *data, const int strip_id,
		const t_image *texture, t_raycast *ray)
{
	if (!texture || !texture->ptr || texture->width <= 0 \
		|| texture->height <= 0)
		return ;
	calculate_and_render_texture_strip(data, texture, ray, strip_id);
}

/*
** Low-level function to get a pointer to a specific pixel in an image
** This function calculates the memory offset for a pixel at coordinates (x, y)
** and returns a pointer to that pixel's memory location
**
** @param image: Image structure containing pixel data
** @param pixel_x: Horizontal pixel coordinate in the image
** @param pixel_y: Vertical pixel coordinate in the image
** @return: Pointer to the pixel's memory location
*/
unsigned int	*get_pixel_ptr(const t_image *image, const int pixel_x, \
					const int pixel_y)
{
	int		pixel_offset;
	char	*dst;

	if (!image || !image->ptr || pixel_x < 0 || pixel_x >= image->width || \
		pixel_y < 0 || pixel_y >= image->height || image->line_length <= 0 || \
		image->bytes_per_pixel <= 0)
		return (NULL);
	pixel_offset = pixel_y * image->line_length + pixel_x * \
		image->bytes_per_pixel;
	if (pixel_offset < 0)
		return (NULL);
	dst = image->ptr + pixel_offset;
	return ((unsigned int *)dst);
}

/*
** Low-level function to set a pixel's color value
** This function writes a color value to a specific pixel location
** in the image buffer
**
** @param image: Image structure to write to
** @param pixel_x: Horizontal pixel coordinate in the image
** @param pixel_y: Vertical pixel coordinate in the image
** @param color: Color value to set (RGBA format)
*/
void	pixel_set(t_image *image, const int pixel_x, const int pixel_y, \
			const int color)
{
	unsigned int	*dst;

	if (pixel_x < 0 || pixel_x >= image->width || pixel_y < 0 || \
		pixel_y >= image->height)
		return ;
	dst = get_pixel_ptr(image, pixel_x, pixel_y);
	if (!dst)
		return ;
	*dst = color;
}
