/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_event.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 13:26:53 by hyamamot          #+#    #+#             */
/*   Updated: 2026/02/20 13:26:53 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raycasting.h"

int	on_key_press(int keycode, t_vars *vars)
{
	if (keycode == KEY_ESC)
	{
		mlx_destroy_window(vars->mlx, vars->win);
		exit(0);
	}
	if (keycode == KEY_W)
		vars->hold_keys[0] = 1;
	if (keycode == KEY_A)
		vars->hold_keys[1] = 1;
	if (keycode == KEY_S)
		vars->hold_keys[2] = 1;
	if (keycode == KEY_D)
		vars->hold_keys[3] = 1;
	if (keycode == KEY_LEFT)
		vars->hold_keys[4] = 1;
	if (keycode == KEY_RIGHT)
		vars->hold_keys[5] = 1;
	return (0);
}

int	on_key_release(int keycode, t_vars *vars)
{
	if (keycode == KEY_ESC)
	{
		mlx_destroy_window(vars->mlx, vars->win);
		exit(0);
	}
	if (keycode == KEY_W)
		vars->hold_keys[0] = 0;
	if (keycode == KEY_A)
		vars->hold_keys[1] = 0;
	if (keycode == KEY_S)
		vars->hold_keys[2] = 0;
	if (keycode == KEY_D)
		vars->hold_keys[3] = 0;
	if (keycode == KEY_LEFT)
		vars->hold_keys[4] = 0;
	if (keycode == KEY_RIGHT)
		vars->hold_keys[5] = 0;
	return (0);
}

int	on_destroy(t_vars *vars)
{
	mlx_destroy_window(vars->mlx, vars->win);
	exit(0);
}
