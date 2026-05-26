/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_deep_copy.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomvaroux <tomvaroux@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 16:46:34 by tomvaroux         #+#    #+#             */
/*   Updated: 2026/03/28 15:42:51 by tomvaroux        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "utils.h"
#include "valid.h"

static int	get_row_width(int *row)
{
	int	width;

	width = 0;
	while (row[width] != -1)
		width++;
	return (width);
}

void	free_map_copy(int **map_copy)
{
	int	i;

	i = 0;
	while (map_copy[i])
	{
		free(map_copy[i]);
		i++;
	}
	free(map_copy);
}

int	map_deep_copy(t_map *map, int ***map_copy, int height)
{
	int	i;
	int	width;

	*map_copy = (int **)malloc(sizeof(int *) * (height + 1));
	if (!*map_copy)
		return (1);
	i = 0;
	while (i < height)
	{
		width = get_row_width(map->map[i]) + 1;
		(*map_copy)[i] = (int *)malloc(sizeof(int) * width);
		if (!(*map_copy)[i])
			return (free_map_copy(*map_copy), 1);
		ft_memcpy((*map_copy)[i], map->map[i], sizeof(int) * width);
		i++;
	}
	(*map_copy)[height] = NULL;
	return (0);
}
