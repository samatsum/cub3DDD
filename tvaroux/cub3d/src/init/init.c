/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 13:26:17 by hyamamot          #+#    #+#             */
/*   Updated: 2026/02/20 13:26:18 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "init.h"
#include "utils.h"

int	is_complete(t_map *data)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (!data->textures[i])
			return (1);
		i++;
	}
	i = 0;
	while (i < 2)
	{
		if (!data->color_set[i])
			return (1);
		i++;
	}
	return (0);
}

int	init(t_map *map, char *infile)
{
	int		fd;
	char	*line;

	fd = open(infile, O_RDONLY);
	if (fd < 0)
		return (perror("open"), 1);
	line = get_next_line(fd);
	map->color_set[0] = 0;
	map->color_set[1] = 0;
	while (line)
	{
		if (!ft_strncmp(line, "NO ", 3) || !ft_strncmp(line, "SO ", 3)
			|| !ft_strncmp(line, "WE ", 3) || !ft_strncmp(line, "EA ", 3))
			set_texture(map, line);
		else if (!ft_strncmp(line, "F ", 2) || !ft_strncmp(line, "C ", 2))
			set_color_code(map, line);
		free(line);
		if (!is_complete(map))
			break ;
		line = get_next_line(fd);
	}
	if (map_init(map, fd))
		return (1);
	close(fd);
	return (0);
}
