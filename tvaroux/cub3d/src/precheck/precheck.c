/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   precheck.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomvaroux <tomvaroux@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 12:44:54 by tomvaroux         #+#    #+#             */
/*   Updated: 2026/03/28 15:41:43 by tomvaroux        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "precheck.h"
#include "utils.h"
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

static int	check_map_part(char *line, t_precheck *p)
{
	if (is_blank_line(line))
		return (p->map_ended = 1, 0);
	if (p->map_ended || !is_map_line(line))
		return (1);
	p->player += count_players(line);
	if (p->player > 1)
		return (1);
	return (0);
}

static int	check_header_part(char *line, t_precheck *p)
{
	if (get_texture_idx(line) >= 0)
		return (check_texture_line(line, p));
	if (get_color_idx(line) >= 0)
		return (check_color_line(line, p));
	return (-1);
}

static int	check_line(char *line, t_precheck *p)
{
	int	ret;

	if (has_tab(line))
		return (1);
	if (!p->map_started && is_blank_line(line))
		return (0);
	if (!p->map_started)
	{
		ret = check_header_part(line, p);
		if (ret != -1)
			return (ret);
		if (!is_map_line(line))
			return (1);
		p->map_started = 1;
	}
	return (check_map_part(line, p));
}

int	precheck_file(char *infile)
{
	int			fd;
	char		*line;
	t_precheck	p;

	fd = open(infile, O_RDONLY);
	if (fd < 0)
		return (printf("Error\nopen failed\n"), 1);
	precheck_init(&p);
	line = get_next_line(fd);
	while (line)
	{
		if (check_line(line, &p))
			return (free(line), close(fd),
				printf("Error\ninvalid .cub file\n"), 1);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	if (finish_precheck(&p))
		return (printf("Error\ninvalid .cub file\n"), 1);
	return (0);
}

// void	precheck_init(t_precheck *p)
// {
// 	ft_memset(p->tex, 0, sizeof(p->tex));
// 	ft_memset(p->color, 0, sizeof(p->color));
// 	p->player = 0;
// 	p->map_started = 0;
// 	p->map_ended = 0;
// }

// int	finish_precheck(t_precheck *p)
// {
// 	int	i;

// 	if (!p->map_started || p->player != 1)
// 		return (1);
// 	i = 0;
// 	while (i < 4)
// 	{
// 		if (!p->tex[i])
// 			return (1);
// 		i++;
// 	}
// 	if (!p->color[0] || !p->color[1])
// 		return (1);
// 	return (0);
// }
