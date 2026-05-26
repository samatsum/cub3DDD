/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   precheck.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomvaroux <tomvaroux@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/28 12:44:30 by tomvaroux         #+#    #+#             */
/*   Updated: 2026/03/28 15:39:40 by tomvaroux        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRECHECK_H
# define PRECHECK_H

# include "main.h"

typedef struct s_precheck
{
	int	tex[4];
	int	color[2];
	int	player;
	int	map_started;
	int	map_ended;
}	t_precheck;

int		get_color_idx(char *line);
int		check_color_line(char *line, t_precheck *p);

void	precheck_init(t_precheck *p);
int		finish_precheck(t_precheck *p);

int		get_texture_idx(char *line);
int		texture_path_is_valid(char *path);	//
int		check_texture_line(char *line, t_precheck *p);

int		has_tab(char *line);
int		is_blank_line(char *line);
int		is_map_line(char *line);
int		count_players(char *line);

int		precheck_file(char *infile);

#endif
