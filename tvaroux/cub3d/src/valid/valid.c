/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomvaroux <tomvaroux@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 14:17:49 by hyamamot          #+#    #+#             */
/*   Updated: 2026/03/28 18:53:54 by tomvaroux        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
// #include "utils.h"
#include "valid.h"

static int	get_height(t_map *map)
{
	int	height;

	height = 0;
	while (map->map[height])
		height++;
	return (height);
}

static int	has_unreachable_floor(int **map_copy, int height)
{
	int	i;
	int	j;

	i = 0;
	while (i < height)
	{
		j = 0;
		while (map_copy[i][j] != -1)
		{
			if (map_copy[i][j] == 0)
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

int	valid(t_map *map)
{
	int		**map_copy;
	bool	out_of_map;
	int		height;
	int		pos[2];

	if (!map->map || !map->map[0])
		return (printf("Error\nempty map\n"), 1);
	height = get_height(map);
	if (map_deep_copy(map, &map_copy, height))
		return (printf("Error\nmalloc failed\n"), 1);
	out_of_map = false;
	pos[0] = map->pos[0];
	pos[1] = map->pos[1];
	dfs(map_copy, pos, &out_of_map, height);
	if (!out_of_map && has_unreachable_floor(map_copy, height))
		out_of_map = true;
	free_map_copy(map_copy);
	if (out_of_map)
		return (printf("Error\ninvalid map\n"), 1);
	return (0);
}

// static int	get_row_width(int *row)
// {
// 	int	width;

// 	width = 0;
// 	while (row[width] != -1)
// 		width++;
// 	return (width);
// }

// static void	free_map_copy(int **map_copy)
// {
// 	int	i;

// 	i = 0;
// 	while (map_copy[i])
// 	{
// 		free(map_copy[i]);
// 		i++;
// 	}
// 	free(map_copy);
// }

// static int	map_deep_copy(t_map *map, int ***map_copy, int height)
// {
// 	int	i;
// 	int	width;

// 	*map_copy = (int **)malloc(sizeof(int *) * (height + 1));
// 	if (!*map_copy)
// 		return (1);
// 	i = 0;
// 	while (i < height)
// 	{
// 		width = get_row_width(map->map[i]) + 1;
// 		(*map_copy)[i] = (int *)malloc(sizeof(int) * width);
// 		if (!(*map_copy)[i])
// 			return (free_map_copy(*map_copy), 1);
// 		ft_memcpy((*map_copy)[i], map->map[i], sizeof(int) * width);
// 		i++;
// 	}
// 	(*map_copy)[height] = NULL;
// 	return (0);
// }
