/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 13:28:15 by hyamamot          #+#    #+#             */
/*   Updated: 2026/02/20 13:28:16 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

static char	*return_line(char *newline_pos, char *line, char *buffer)
{
	char	*new_line;

	*newline_pos = '\0';
	new_line = ft_strjoin(line, buffer);
	free(line);
	ft_memmove(buffer, newline_pos + 1, ft_strlen(newline_pos + 1) + 1);
	return (new_line);
}

static int	fill_buffer(int fd, char *buffer)
{
	int	read_bytes;

	read_bytes = read(fd, buffer, BUFFER_SIZE);
	if (read_bytes <= 0)
		return (0);
	buffer[read_bytes] = '\0';
	return (1);
}

char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE + 1];
	char		*line;
	char		*newline_pos;
	char		*tmp;

	line = NULL;
	if (fd <= -1 || BUFFER_SIZE <= 0)
		return (NULL);
	while (1)
	{
		if (!(*buffer) && !fill_buffer(fd, buffer))
			return (line);
		newline_pos = ft_strchr(buffer, '\n');
		if (newline_pos)
			return (return_line(newline_pos, line, buffer));
		tmp = line;
		line = ft_strjoin(line, buffer);
		free(tmp);
		*buffer = '\0';
	}
}
