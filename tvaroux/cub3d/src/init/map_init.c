/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 13:26:21 by hyamamot          #+#    #+#             */
/*   Updated: 2026/02/20 13:26:22 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "init.h"
#include "utils.h"

static void	map_add_row(t_map *map, int i, int len, char *line)
{
	int	j;

	map->map = ft_realloc(map->map, sizeof(int *) * i, sizeof(int *) * (i + 1));
	map->map[i] = (int *)malloc(sizeof(int) * (len + 1));
	j = 0;
	while (j < len)
	{
		if (line[j] == '0' || line[j] == '1')
			map->map[i][j] = line[j] - '0';
		else if (line[j] == 'N' || line[j] == 'S'
			|| line[j] == 'E' || line[j] == 'W')
		{
			map->pos[0] = i;
			map->pos[1] = j;
			map->dir = line[j];
			map->map[i][j] = 0;
		}
		else
			map->map[i][j] = 2;
		j++;
	}
	map->map[i][len] = -1;
}

int	null_termination(t_map *map, int i)
{
	map->map = ft_realloc(map->map, sizeof(int *) * i, sizeof(int *) * (i + 1));
	if (!map->map)
		return (1);
	map->map[i] = NULL;
	return (0);
}

int	map_init(t_map *map, int fd)
{
	char	*line;
	int		i;
	int		len;

	i = 0;
	line = get_next_line(fd);
	while (line)
	{
		len = ft_strlen(line);
		if (ft_strchr(line, '\n'))
			len -= 1;
		if (len == 0)
		{
			free(line);
			line = get_next_line(fd);
			continue ;
		}
		map_add_row(map, i, len, line);
		free(line);
		i++;
		line = get_next_line(fd);
	}
	if (null_termination(map, i))
		return (1);
	return (0);
}
