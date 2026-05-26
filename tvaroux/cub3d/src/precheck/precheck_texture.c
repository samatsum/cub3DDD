/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   precheck_texture.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomvaroux <tomvaroux@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 12:45:48 by tomvaroux         #+#    #+#             */
/*   Updated: 2026/03/28 15:24:41 by tomvaroux        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "precheck.h"
#include "utils.h"

int	get_texture_idx(char *line)
{
	if (!ft_strncmp(line, "NO ", 3))
		return (0);
	if (!ft_strncmp(line, "SO ", 3))
		return (1);
	if (!ft_strncmp(line, "WE ", 3))
		return (2);
	if (!ft_strncmp(line, "EA ", 3))
		return (3);
	return (-1);
}

static int	get_path_end(char *path)
{
	int	end;

	end = 0;
	while (path[end] && path[end] != '\n')
		end++;
	while (end > 0 && path[end - 1] == ' ')
		end--;
	return (end);
}

static int	has_space_in_path(char *path, int end)
{
	int	i;

	i = 0;
	while (i < end)
	{
		if (path[i] == ' ')
			return (1);
		i++;
	}
	return (0);
}

int	texture_path_is_valid(char *path)
{
	int	end;

	end = get_path_end(path);
	if (end < 4)
		return (0);
	if (has_space_in_path(path, end))
		return (0);
	if (ft_strncmp(path + end - 4, ".xpm", 4) != 0)
		return (0);
	return (1);
}

int	check_texture_line(char *line, t_precheck *p)
{
	int		idx;
	char	*path;

	idx = get_texture_idx(line);
	if (idx < 0 || p->tex[idx])
		return (1);
	path = line + 2;
	while (*path == ' ')
		path++;
	if (*path == '\0' || *path == '\n')
		return (1);
	if (!texture_path_is_valid(path))
		return (1);
	p->tex[idx] = 1;
	return (0);
}

// #include "precheck.h"
// #include "utils.h"

// int	get_texture_idx(char *line)
// {
// 	if (!ft_strncmp(line, "NO ", 3))
// 		return (0);
// 	if (!ft_strncmp(line, "SO ", 3))
// 		return (1);
// 	if (!ft_strncmp(line, "WE ", 3))
// 		return (2);
// 	if (!ft_strncmp(line, "EA ", 3))
// 		return (3);
// 	return (-1);
// }

// int	check_texture_line(char *line, t_precheck *p)
// {
// 	int		idx;
// 	char	*path;

// 	idx = get_texture_idx(line);
// 	if (idx < 0 || p->tex[idx])
// 		return (1);
// 	path = line + 2;
// 	while (*path == ' ')
// 		path++;
// 	if (*path == '\0' || *path == '\n')
// 		return (1);
// 	p->tex[idx] = 1;
// 	return (0);
// }
