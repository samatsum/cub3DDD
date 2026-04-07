/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_control.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: torinoue <torinoue@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 21:03:45 by torinoue          #+#    #+#             */
/*   Updated: 2025/11/06 20:54:30 by torinoue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static void	debug_print_player_angle(const double *angle);

void	view_rotation(double *angle, int direction)
{
	double	half_rotation_value;

	if (direction == RIGHT)
		*angle += DEFAULT_ROTATE_VALUE;
	else
		*angle -= DEFAULT_ROTATE_VALUE;
	*angle = normalize_angle(*angle);
	half_rotation_value = DEFAULT_ROTATE_VALUE / 2.0;
	if (*angle <= half_rotation_value || \
		*angle >= 2 * PI - half_rotation_value)
		*angle = 0;
	else if (*angle >= PI / 2 - half_rotation_value && \
			*angle <= PI / 2 + half_rotation_value)
		*angle = PI / 2;
	else if (*angle >= PI - half_rotation_value && \
			*angle <= PI + half_rotation_value)
		*angle = PI;
	else if (*angle >= 3 * PI / 2 - half_rotation_value && \
			*angle <= 3 * PI / 2 + half_rotation_value)
		*angle = 3 * PI / 2;
	debug_print_player_angle(angle);
}

static void	debug_print_player_angle(const double *angle)
{
	ft_printf_fd(INFO_FD, "player.angle: ");
	print_angle_formatted_fd(INFO_FD, *angle);
	ft_printf_fd(INFO_FD, "\n");
}

/**
 * @brief Handler for left rotation (LEFT arrow key)
 * @param key_ctx Key event context containing player and map context
 */
void	handle_rotate_left(t_key_event_context *key_ctx)
{
	view_rotation(&key_ctx->player->angle, LEFT);
}

/**
 * @brief Handler for right rotation (RIGHT arrow key)
 * @param key_ctx Key event context containing player and map context
 */
void	handle_rotate_right(t_key_event_context *key_ctx)
{
	view_rotation(&key_ctx->player->angle, RIGHT);
}
