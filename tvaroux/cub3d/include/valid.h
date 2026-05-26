/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomvaroux <tomvaroux@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 15:10:44 by hyamamot          #+#    #+#             */
/*   Updated: 2026/03/28 15:29:36 by tomvaroux        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VALID_H
# define VALID_H

# include "main.h"
# include <stdbool.h>
# include <stdlib.h>

int		valid(t_map *map);

void	free_map_copy(int **map_copy);
int		map_deep_copy(t_map *map, int ***map_copy, int height);

void	dfs(int **map, int pos[2], bool *out_of_map, int height);

#endif
