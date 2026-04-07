/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_events_hooks.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: torinoue <torinoue@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 23:35:07 by torinoue          #+#    #+#             */
/*   Updated: 2025/10/29 22:07:44 by torinoue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

// int	mouse_press_event(int button, int x, int y, t_data *data)
// {
// 	(void)x;
// 	(void)y;
// 	if (button == MOUSE_WHEEL_UP)
// 		view_rotation(&data->player.angle, LEFT);
// 	else if (button == MOUSE_WHEEL_DOWN)
// 		view_rotation(&data->player.angle, RIGHT);
// 	return (0);
// }

int	mouse_press_event(int button, int x, int y, double *angle)
{
	(void)x;
	(void)y;
	if (button == MOUSE_WHEEL_UP)
		view_rotation(angle, LEFT);
	else if (button == MOUSE_WHEEL_DOWN)
		view_rotation(angle, RIGHT);
	return (0);
}
