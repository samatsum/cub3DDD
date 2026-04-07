/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_loader.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: torinoue <torinoue@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 00:00:00 by torinoue          #+#    #+#             */
/*   Updated: 2025/11/07 00:00:00 by torinoue        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d.h"

static void	read_map_data(t_data *data, const int fd);

/*
** Loads the map data from the scene file into the map context
** Allocates memory for the map array and reads map lines
**
** @param data: Game data structure containing map context and filepath
*/
void	load_map(t_data *data)
{
	int		fd;

	fd = open(data->scene_filepath, O_RDONLY);
	if (fd == -1)
		display_error(FILE_ERROR, data);
	data->map_ctx.map = (char **)malloc(sizeof(char *) * \
	(data->map_ctx.map_y_size + 1));
	data->map_ctx.map_x_size = (int *)ft_calloc(data->map_ctx.map_y_size + 1, \
		sizeof(int));
	if (!data->map_ctx.map || !data->map_ctx.map_x_size)
		display_error(INTERNAL_ERROR, data);
	read_map_data(data, fd);
	get_next_line(fd, true);
	close(fd);
}

/*
** Reads map lines from file and stores them in the map context
** Only processes lines starting with '1' or ' ' (map content)
**
** @param data: Game data structure containing map context
** @param fd: File descriptor of the scene file
*/
static void	read_map_data(t_data *data, const int fd)
{
	char	*line;
	int		i;
	int		len;

	i = 0;
	line = get_next_line(fd, false);
	while (line)
	{
		if (line[0] == '1' || line[0] == ' ')
		{
			data->map_ctx.map[i] = ft_strdup(line);
			len = ft_strlen(line);
			data->map_ctx.map_x_size[i++] = len - (line[len - 1] == '\n');
		}
		free(line);
		line = get_next_line(fd, false);
	}
	data->map_ctx.map[i] = NULL;
	data->map_ctx.max_coord_y = data->map_ctx.map_y_size * \
	data->map_ctx.tile_size;
}
