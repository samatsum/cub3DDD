/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toruinoue <toruinoue@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 12:14:14 by torinoue          #+#    #+#             */
/*   Updated: 2025/10/26 01:29:53 by toruinoue        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdbool.h>

static int	get_newline_pos(char *str);
static char	*read_and_store(char *str, int fd, int *bytes_read);
static char	*extract_line(char **storage_ptr, int pos);
static char	*process_line(char **storage, int newline_pos);

char	*get_next_line(int fd, bool is_free)
{
	char			*line;
	static char		*storage[OPEN_MAX + 1];
	int				newline_pos;
	int				bytes_read[1];

	if (BUFFER_SIZE <= 0 || BUFFER_SIZE + 1 > SIZE_MAX || is_free)
		return (free_and_rest_memory(&storage[fd]));
	if (fd < 0 || OPEN_MAX < fd)
		return (NULL);
	newline_pos = get_newline_pos(storage[fd]);
	*bytes_read = LOOP_CONDITION_INIT_VALUE;
	while (newline_pos == 0 && *bytes_read > 0)
	{
		storage[fd] = read_and_store(storage[fd], fd, bytes_read);
		if (storage[fd] == NULL)
			return (free_and_rest_memory(&storage[fd]));
		newline_pos = get_newline_pos(storage[fd]);
	}
	line = process_line(&storage[fd], newline_pos);
	return (line);
}

static char	*process_line(char **storage, int newline_pos)
{
	char	*line;

	line = NULL;
	if (newline_pos)
		line = extract_line(storage, newline_pos);
	else if (gnl_strlen(*storage))
	{
		line = gnl_strdup(*storage);
		free_and_rest_memory(storage);
	}
	else
		free_and_rest_memory(storage);
	return (line);
}

static int	get_newline_pos(char *str)
{
	size_t	i;

	if (str == NULL)
		return (0);
	i = 0;
	while (str[i])
	{
		if (str[i] == '\n')
			return (i + 1);
		i++;
	}
	return (0);
}

static char	*read_and_store(char *str, int fd, int *bytes_read)
{
	char	*buf;
	char	*buf_joined;

	buf = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (buf == NULL)
		return (free_and_rest_memory(&str));
	*bytes_read = read(fd, buf, BUFFER_SIZE);
	if (*bytes_read < 0)
	{
		free_and_rest_memory(&buf);
		return (free_and_rest_memory(&str));
	}
	buf[*bytes_read] = '\0';
	buf_joined = gnl_strjoin(str, buf);
	free_and_rest_memory(&str);
	free_and_rest_memory(&buf);
	return (buf_joined);
}

static char	*extract_line(char **storage_ptr, int newline_pos)
{
	char	*line;
	char	*temp;
	int		i;

	line = (char *)malloc(sizeof(char) * (newline_pos + 1));
	if (line == NULL)
		return (free_and_rest_memory(storage_ptr));
	i = 0;
	while (i < newline_pos)
	{
		line[i] = (*storage_ptr)[i];
		i++;
	}
	line[newline_pos] = '\0';
	temp = *storage_ptr;
	*storage_ptr = gnl_strdup(&temp[newline_pos]);
	free_and_rest_memory(&temp);
	if (*storage_ptr == NULL)
		return (NULL);
	return (line);
}

/*
 #include <fcntl.h>
 #include <stdio.h>
 int main()
 {
 	char *test = "";
 	int fd1 = open("test1", O_RDONLY);
	int fd2 = open("test2", O_RDONLY);
 	while (test)
 	{
 		test = get_next_line(fd1);
 		printf("fd1:%s", test);
 		free(test);
 		test = get_next_line(fd2);
 		printf("fd2:%s", test);
 		free(test);
 	}
 	close(fd1);
 	close(fd2);
 }

__attribute__((destructor))
static void destructor() {
	system("leaks -q a.out");
}
*/
/*
cc get_next_line_bonus.c get_next_line_utils_bonus.c get_next_line_bonus.h
 -Wall -Wextra -Werror -D BUFFER_SIZE=4 && ./a.out |cat -e
*/