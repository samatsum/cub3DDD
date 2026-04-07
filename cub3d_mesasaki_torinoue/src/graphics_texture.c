/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics_texture.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toruinoue <toruinoue@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 21:00:00 by torinoue          #+#    #+#             */
/*   Updated: 2025/11/09 01:12:53 by toruinoue        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

/*
** Graphics texture rendering module
** This module handles texture coordinate calculations and pixel rendering
** for vertical strips in the 3D raycasting rendering pipeline
*/

static int	calculate_texture_x_coord(const t_raycast *ray, \
	const t_map_context *map_ctx, const t_image *texture);
static void	render_strip_pixel_loop(t_image *screen, const t_image *texture, \
	t_raycast *ray, const t_render_params *params);

/*
** Calculates texture X coordinate and renders the vertical strip
** This is a helper function that combines texture coordinate calculation
** and pixel rendering for a vertical strip
**
** Note: This function takes t_data *data instead of individual members
** (data->screen and data->map_ctx) due to Norminette's limit of maximum
** 4 function parameters. If we passed individual members, we would need:
** 1. t_image *screen, 2. const t_map_context *map_ctx,
** 3. const t_image *texture, 4. t_raycast *ray, 5. int strip_id
** (5 parameters, exceeding the limit).
**
** @param data: Game data containing screen and tile information
** @param texture: Texture image to map onto the wall
** @param ray: Raycast data containing intersection and wall information
** @param strip_id: Horizontal position of the vertical strip
*/
void	calculate_and_render_texture_strip(t_data *data,
		const t_image *texture, t_raycast *ray, const int strip_id)
{
	t_render_params		params;
	const t_map_context	*map_ctx;

	map_ctx = &data->map_ctx;
	params.strip_id = strip_id;
	params.x_coord = calculate_texture_x_coord(ray, map_ctx, texture);
	render_strip_pixel_loop(&data->screen, texture, ray, &params);
}

/*
** Calculates the X coordinate in the texture for the given ray intersection
** Handles texture flipping based on wall side and ray direction
**
** @param ray: Raycast data containing intersection and direction information
** @param map_ctx: Map context containing tile size information
** @param texture: Texture image to map onto the wall
** @return: X coordinate in texture (0 to texture->width - 1)
*/
static int	calculate_texture_x_coord(const t_raycast *ray,
		const t_map_context *map_ctx, const t_image *texture)
{
	int	tex_x;
	int	x_coord_texture;

	tex_x = (ray->hit_side == HORIZONTAL) * ((int)ray->intersection.x % \
	map_ctx->tile_size) + (ray->hit_side != HORIZONTAL) * \
		((int)ray->intersection.y % map_ctx->tile_size);
	if (tex_x < 0)
		tex_x += map_ctx->tile_size;
	x_coord_texture = tex_x * texture->width * map_ctx->tile_size_inv;
	(ray->hit_side == HORIZONTAL && ray->v_direction == DOWN) &&
	(x_coord_texture = texture->width - x_coord_texture - 1);
	(ray->hit_side == VERTICAL && ray->h_direction == LEFT) &&
	(x_coord_texture = texture->width - x_coord_texture - 1);
	if (x_coord_texture < 0)
		x_coord_texture = 0;
	if (x_coord_texture >= texture->width)
		x_coord_texture = texture->width - 1;
	return (x_coord_texture);
}

/*
** Renders pixels for a vertical strip by iterating through wall height
** Calculates texture Y coordinate and samples pixel from texture
**
** @param screen: Screen image buffer
** @param texture: Texture image to map onto the wall
** @param ray: Raycast data containing wall height and texture position
** @param params: Render parameters containing strip_id and x_coord
*/
static void	render_strip_pixel_loop(t_image *screen, const t_image *texture,
		t_raycast *ray, const t_render_params *params)
{
	int				y;
	unsigned int	*pixel_ptr;
	int				tex_y;

	y = ray->wall_top;
	while (y <= ray->wall_bottom)
	{
		tex_y = (int)ray->tex_y_pos;
		if (tex_y < 0)
			tex_y = 0;
		else if (tex_y >= texture->height)
			tex_y = texture->height - 1;
		ray->tex_y_pos += ray->tex_y_step;
		pixel_ptr = get_pixel_ptr(texture, params->x_coord, tex_y);
		if (pixel_ptr)
			pixel_set(screen, params->strip_id, y, *pixel_ptr);
		y++;
	}
}
