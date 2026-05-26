/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 15:10:25 by hyamamot          #+#    #+#             */
/*   Updated: 2026/02/20 15:10:26 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H

# include <stdio.h>
# include <string.h>
# include <fcntl.h>
# include <unistd.h>

typedef struct s_map
{
	char	*textures[4];
	int		color_code[2];
	int		color_set[2];
	int		**map;
	int		pos[2];
	char	dir;
}	t_map;

#endif
