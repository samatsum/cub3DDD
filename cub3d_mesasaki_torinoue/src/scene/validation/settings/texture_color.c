/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_texture_color_validation.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: torinoue <torinoue@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 00:45:00 by toruinoue         #+#    #+#             */
/*   Updated: 2025/11/03 21:21:32 by torinoue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/cub3d.h"

static bool	check_texture_line(const char *line, void *texture_ctx);
static bool	is_invalid_texture_path(const char *line);
static bool	check_color_line(const char *line, void *color_ctx);

bool	is_invalid_texture_setting(const char *filepath, t_data *data)
{
	bool	res;

	res = false;
	check_lines_with_callback(filepath, \
		check_texture_line, &res, data);
	return (res);
}

static bool	check_texture_line(const char *line, void *texture_ctx)
{
	bool	*res;

	if (ft_strncmp(line, "NO ", 3) == 0 || ft_strncmp(line, "SO ", 3) == 0
		|| ft_strncmp(line, "WE ", 3) == 0 || ft_strncmp(line, "EA ", 3) == 0)
	{
		res = (bool *)texture_ctx;
		*res = is_invalid_texture_path(line);
		return (*res);
	}
	return (false);
}

static bool	is_invalid_texture_path(const char *line)
{
	char	*path;
	bool	res;

	res = false;
	path = ft_strtrim(line + 3, " \n");
	if (!path || !*path)
		res = true;
	else if (access(path, R_OK) != 0 || !ft_strnstr(path, ".xpm",
			ft_strlen(path)))
		res = true;
	free(path);
	return (res);
}

bool	is_invalid_color_setting(const char *filepath, t_data *data)
{
	bool	res;

	res = false;
	check_lines_with_callback(filepath, \
		check_color_line, &res, data);
	return (res);
}

static bool	check_color_line(const char *line, void *color_ctx)
{
	bool	*res;

	if (line && (ft_strncmp(line, "F ", 2) == 0
			|| ft_strncmp(line, "C ", 2) == 0)
		&& ft_strlen(line) > 2)
	{
		res = (bool *)color_ctx;
		*res = is_invalid_color_value(line + 2);
		return (*res);
	}
	return (false);
}
