/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   precheck_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomvaroux <tomvaroux@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 12:45:21 by tomvaroux         #+#    #+#             */
/*   Updated: 2026/03/28 12:45:23 by tomvaroux        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "precheck.h"

int	has_tab(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '\t')
			return (1);
		i++;
	}
	return (0);
}

int	is_blank_line(char *line)
{
	int	i;

	i = 0;
	while (line[i] && line[i] != '\n')
	{
		if (line[i] != ' ')
			return (0);
		i++;
	}
	return (1);
}

int	is_map_line(char *line)
{
	int	i;
	int	has_tile;

	i = 0;
	has_tile = 0;
	while (line[i] && line[i] != '\n')
	{
		if (line[i] != ' ' && line[i] != '0' && line[i] != '1'
			&& line[i] != 'N' && line[i] != 'S'
			&& line[i] != 'E' && line[i] != 'W')
			return (0);
		if (line[i] != ' ')
			has_tile = 1;
		i++;
	}
	return (has_tile);
}

int	count_players(char *line)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (line[i] && line[i] != '\n')
	{
		if (line[i] == 'N' || line[i] == 'S'
			|| line[i] == 'E' || line[i] == 'W')
			count++;
		i++;
	}
	return (count);
}
