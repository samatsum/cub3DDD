/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_validation_file_helper.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toruinoue <toruinoue@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 00:45:00 by toruinoue          #+#    #+#            */
/*   Updated: 2025/11/03 00:45:00 by toruinoue        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_types.h"
#include "../../../include/cub3d_functions.h"
#include "../../../lib/libft/get_next_line.h"
#include <fcntl.h>
#include <unistd.h>

/**
 * @brief Checks lines in a file using a callback function
 * 
 * This function abstracts file I/O operations, allowing validation logic
 * to focus solely on line content validation. It handles file opening,
 * line reading, error handling, and resource cleanup.
 * 
 * @param filepath Path to the file to check
 * @param callback Function to check each line. Returns true if check fails.
 * @param context Context data passed to the callback function
 * @return true if check fails (callback returns true for any line)
 */
bool	check_lines_with_callback(const char *filepath, \
	bool (*callback)(const char *line, void *context), void *context, \
	t_data *data)
{
	char	*line;
	int		fd;
	bool	res;

	res = false;
	line = NULL;
	fd = open(filepath, O_RDONLY);
	if (fd == -1)
		display_error(FILE_ERROR, data);
	while (!res)
	{
		line = get_next_line(fd, false);
		if (!line)
			break ;
		res = callback(line, context);
		free(line);
	}
	close(fd);
	get_next_line(fd, true);
	return (res);
}
/*
template<typename Callback>
bool check_lines_with_callback(const std::string& filepath, Callback&& callback);
*/