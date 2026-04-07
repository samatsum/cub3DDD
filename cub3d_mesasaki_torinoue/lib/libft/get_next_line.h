/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toruinoue <toruinoue@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 10:59:08 by torinoue          #+#    #+#             */
/*   Updated: 2025/10/26 01:29:53 by toruinoue        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# define LOOP_CONDITION_INIT_VALUE 1
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <stddef.h>
# include <stdbool.h>
# include <stdint.h>

# ifndef OPEN_MAX
#  define OPEN_MAX 1024
# endif

char	*get_next_line(int fd, bool is_free);
char	*free_and_rest_memory(char **memmory);
char	*gnl_strjoin(char const *s1, char const *s2);
char	*gnl_strdup(const char *src);
size_t	gnl_strlen(const char *s);

#endif