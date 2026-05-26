/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dfs.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomvaroux <tomvaroux@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 14:17:54 by hyamamot          #+#    #+#             */
/*   Updated: 2026/03/28 18:53:45 by tomvaroux        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "valid.h"

static int	get_row_width(int *row)
{
	int	width;

	width = 0;
	while (row[width] != -1)
		width++;
	return (width);
}

static int	get_cell(int **map, int row, int col, int height)
{
	int	width;

	if (row < 0 || row >= height)
		return (2);
	width = get_row_width(map[row]);
	if (col < 0 || col >= width)
		return (2);
	return (map[row][col]);
}

void	dfs(int **map, int pos[2], bool *out_of_map, int height)
{
	int			i;
	int			npos[2];
	int			cell;
	static int	move[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

	if (*out_of_map)
		return ;
	cell = get_cell(map, pos[0], pos[1], height);
	if (cell == 1 || cell == 3)
		return ;
	if (cell == 2)
		return (*out_of_map = true, (void)0);
	map[pos[0]][pos[1]] = 3;
	i = 0;
	while (i < 4 && !*out_of_map)
	{
		npos[0] = pos[0] + move[i][0];
		npos[1] = pos[1] + move[i][1];
		dfs(map, npos, out_of_map, height);
		i++;
	}
}
