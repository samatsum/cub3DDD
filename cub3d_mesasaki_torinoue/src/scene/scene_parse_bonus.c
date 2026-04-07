/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_parse_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: torinoue <torinoue@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 23:09:26 by torinoue          #+#    #+#             */
/*   Updated: 2025/11/13 17:04:58 by torinoue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static void	parse_line(t_data *data, char *line, const int fd);
static void	parse_color_line(const char *line, int *color, t_data *data);
static int	calculate_collision_ceiling_color(const int base_ceiling_color);

void	parse_scene_settings(t_data *data)
{
	char	*line;
	int		fd;

	line = NULL;
	fd = open(data->scene_filepath, O_RDONLY);
	if (fd == -1)
		display_error(FILE_ERROR, data);
	line = get_next_line(fd, false);
	while (line)
	{
		parse_line(data, line, fd);
		free(line);
		line = get_next_line(fd, false);
	}
	get_next_line(fd, true);
	close(fd);
}

static void	parse_line(t_data *data, char *line, const int fd)
{
	if (ft_strncmp(line, "NO ", 3) == 0)
		data->tex[TEX_NORTH].path = ft_strtrim(line + 3, " \n");
	else if (ft_strncmp(line, "SO ", 3) == 0)
		data->tex[TEX_SOUTH].path = ft_strtrim(line + 3, " \n");
	else if (ft_strncmp(line, "WE ", 3) == 0)
		data->tex[TEX_WEST].path = ft_strtrim(line + 3, " \n");
	else if (ft_strncmp(line, "EA ", 3) == 0)
		data->tex[TEX_EAST].path = ft_strtrim(line + 3, " \n");
	else if (ft_strncmp(line, "WC ", 3) == 0)
		data->tex[TEX_COLLISION].path = ft_strtrim(line + 3, " \n");
	else if (ft_strncmp(line, "F ", 2) == 0)
		parse_color_line(line, &data->floor_color, data);
	else if (ft_strncmp(line, "C ", 2) == 0)
	{
		parse_color_line(line, &data->game.ceiling_color, data);
		data->original_ceiling_color = data->game.ceiling_color;
		data->collision_ceiling_color = calculate_collision_ceiling_color(
				data->game.ceiling_color);
	}
	else if (line[0] == '\n')
		return ;
	else if (line[0] == '1' || line[0] == ' ')
		data->map_ctx.map_y_size++;
	else if (free(line), get_next_line(fd, true), 1)
		display_setting_error(INVALID_IDENTIFIER, data);
}

static void	parse_color_line(const char *line, int *color, t_data *data)
{
	t_rgb	rgb;
	char	**tmp;

	tmp = ft_split(line + 2, ',');
	if (!tmp || arrlen(tmp) < 3)
	{
		if (tmp)
			free_str_array(&tmp);
		display_setting_error(INVALID_COLOR_SETTING, data);
		return ;
	}
	if (is_invalid_color_value(line + 2))
	{
		free_str_array(&tmp);
		display_setting_error(INVALID_COLOR_VALUE, data);
		return ;
	}
	rgb.r = ft_atoi(tmp[0]);
	rgb.g = ft_atoi(tmp[1]);
	rgb.b = ft_atoi(tmp[2]);
	*color = rgb_to_hex(rgb);
	free_str_array(&tmp);
}

/*
** Calculates collision ceiling color from base ceiling color
** Applies ON_WALL_CEILING_BRIGHTNESS to the base color
** This is a cub3D-specific function for collision visual feedback
**
** @param base_ceiling_color: Base ceiling color in hexadecimal format
** @return: Collision ceiling color with brightness applied
*/
static int	calculate_collision_ceiling_color(const int base_ceiling_color)
{
	return (rgb_to_hex_with_brightness(hex_to_rgb(base_ceiling_color),
			ON_WALL_CEILING_BRIGHTNESS));
}
