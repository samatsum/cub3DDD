/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   content_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 18:08:29 by hyamamot          #+#    #+#             */
/*   Updated: 2026/02/20 18:08:30 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "utils.h"

void	set_texture(t_map *map, char *line)
{
	char	*start;
	char	*p;

	start = line + 2;
	while (ft_isspace(*start))
		start++;
	p = ft_strchr(start, '\n');
	if (p)
		*p = '\0';
	if (!ft_strncmp(line, "NO ", 3))
		map->textures[0] = ft_strdup(start);
	else if (!ft_strncmp(line, "SO ", 3))
		map->textures[1] = ft_strdup(start);
	else if (!ft_strncmp(line, "WE ", 3))
		map->textures[2] = ft_strdup(start);
	else if (!ft_strncmp(line, "EA ", 3))
		map->textures[3] = ft_strdup(start);
}

static void	free_rgb(char **rgb)
{
	free(rgb[0]);
	free(rgb[1]);
	free(rgb[2]);
	free(rgb);
}

static int	valid_color(char **rgb)
{
	int	i;
	int	elem;

	i = 0;
	while (rgb[i])
	{
		elem = ft_atoi(rgb[i]);
		if (elem < 0 || elem > 255)
			return (1);
		i++;
	}
	if (i != 3)
		return (1);
	return (0);
}

void	parse_rgb_val(char *line, int *r, int *g, int *b)
{
	char	*start;
	char	**rgb;

	start = line + 1;
	while (ft_isspace(*start))
		start++;
	rgb = ft_split(start, ',');
	if (valid_color(rgb))
		return ;
	*r = ft_atoi(rgb[0]);
	*g = ft_atoi(rgb[1]);
	*b = ft_atoi(rgb[2]);
	free_rgb(rgb);
}

void	set_color_code(t_map *map, char *line)
{
	int		r;
	int		g;
	int		b;

	parse_rgb_val(line, &r, &g, &b);
	if (line[0] == 'F')
	{
		map->color_code[0] = (r << 16) | (g << 8) | b;
		map->color_set[0] = 1;
	}
	else if (line[0] == 'C')
	{
		map->color_code[1] = (r << 16) | (g << 8) | b;
		map->color_set[1] = 1;
	}
}
