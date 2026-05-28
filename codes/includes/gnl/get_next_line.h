/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 15:19:57 by samatsum          #+#    #+#             */
/*   Updated: 2026/05/28 21:10:25 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>
# include "utils/utils.h"

# define BUFFER_SIZE 128

typedef struct	s_fd
{
	int			fd;
	t_str		*str;
	struct s_fd	*next;
}				t_fd;

int				get_next_line(int fd, char **line);

t_fd			*find_fd(t_fd **list, int fd, int *new);

int				find_nl(t_str *str, char *sim_str);

int				read_file(t_str **str, char *buffer, int fd);

#endif
