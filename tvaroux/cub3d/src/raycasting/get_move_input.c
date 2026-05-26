/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_move_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 13:26:38 by hyamamot          #+#    #+#             */
/*   Updated: 2026/02/20 13:26:38 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raycasting.h"

static void	move_straight(t_vars *vars, double spd, int sign)
{
	double	dx;
	double	dy;
	int		nx;
	int		ny;

	dx = vars->dir_x * spd * sign;
	dy = vars->dir_y * spd * sign;
	nx = (int)(vars->pos_x + dx);
	if (vars->map[(int)vars->pos_y][nx] == 0)
		vars->pos_x += dx;
	ny = (int)(vars->pos_y + dy);
	if (vars->map[ny][(int)vars->pos_x] == 0)
		vars->pos_y += dy;
}

static void	move_strafe(t_vars *vars, double spd, int sign)
{
	double	rdx;
	double	rdy;
	int		nx;
	int		ny;

	rdx = vars->dir_y * sign;
	rdy = -vars->dir_x * sign;
	nx = (int)(vars->pos_x + rdx * spd);
	if (vars->map[(int)vars->pos_y][nx] == 0)
		vars->pos_x += rdx * spd;
	ny = (int)(vars->pos_y + rdy * spd);
	if (vars->map[ny][(int)vars->pos_x] == 0)
		vars->pos_y += rdy * spd;
}

static void	rotate(t_vars *vars, double spd)
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = vars->dir_x;
	vars->dir_x = old_dir_x * cos(spd) - vars->dir_y * sin(spd);
	vars->dir_y = old_dir_x * sin(spd) + vars->dir_y * cos(spd);
	old_plane_x = vars->plane_x;
	vars->plane_x = old_plane_x * cos(spd) - vars->plane_y * sin(spd);
	vars->plane_y = old_plane_x * sin(spd) + vars->plane_y * cos(spd);
}

int	get_move_input(t_vars *vars)
{
	double	old_time;
	double	frame_time;
	double	move_spd;
	double	rot_spd;

	old_time = vars->time_ms;
	vars->time_ms = get_time_ms();
	frame_time = (vars->time_ms - old_time) / 1000.0;
	move_spd = frame_time * 5.0;
	rot_spd = frame_time * 3.0;
	if (vars->hold_keys[0])
		move_straight(vars, move_spd, 1);
	if (vars->hold_keys[1])
		move_strafe(vars, move_spd, 1);
	if (vars->hold_keys[2])
		move_straight(vars, move_spd, -1);
	if (vars->hold_keys[3])
		move_strafe(vars, move_spd, -1);
	if (vars->hold_keys[4])
		rotate(vars, -rot_spd);
	if (vars->hold_keys[5])
		rotate(vars, rot_spd);
	return (0);
}
