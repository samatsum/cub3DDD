/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 15:10:21 by hyamamot          #+#    #+#             */
/*   Updated: 2026/02/20 15:10:22 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INIT_H
# define INIT_H

# include "main.h"

int		init(t_map *map, char *infile);
int		map_init(t_map *map, int fd);
void	map_free(t_map *map);
void	set_color_code(t_map *map, char *line);
void	set_texture(t_map *map, char *line);

#endif
