/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_rgb.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: torinoue <torinoue@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 21:03:45 by torinoue          #+#    #+#             */
/*   Updated: 2025/11/03 21:22:26 by torinoue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

int	rgb_to_hex(const t_rgb rgb)
{
	return ((rgb.r << 16) | (rgb.g << 8) | rgb.b);
}

/*
** Converts hexadecimal color value to RGB structure
**
** @param hex: Hexadecimal color value (0xRRGGBB format)
** @return: RGB color structure
*/
t_rgb	hex_to_rgb(const int hex)
{
	t_rgb	rgb;

	rgb.r = (hex >> 16) & 0xFF;
	rgb.g = (hex >> 8) & 0xFF;
	rgb.b = hex & 0xFF;
	return (rgb);
}

/*
** Converts RGB color to hexadecimal with brightness adjustment
** Applies brightness multiplier to each RGB component and clamps to 0-255 range
**
** @param rgb: RGB color structure (r, g, b values 0-255)
** @param brightness: Brightness multiplier
**   (0.0 = black, 1.0 = original, >1.0 = brighter)
** @return: Hexadecimal color value with brightness applied
*/
int	rgb_to_hex_with_brightness(const t_rgb rgb, const double brightness)
{
	int	r;
	int	g;
	int	b;

	r = (int)(rgb.r * brightness);
	g = (int)(rgb.g * brightness);
	b = (int)(rgb.b * brightness);
	if (r < 0)
		r = 0;
	else if (r > 255)
		r = 255;
	if (g < 0)
		g = 0;
	else if (g > 255)
		g = 255;
	if (b < 0)
		b = 0;
	else if (b > 255)
		b = 255;
	return ((r << 16) | (g << 8) | b);
}
