/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_events_handlers_movement.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: torinoue <torinoue@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 15:00:00 by torinoue          #+#    #+#             */
/*   Updated: 2025/11/09 15:00:31 by torinoue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static void	handle_move_forward(t_key_event_context *key_ctx);
static void	handle_move_backward(t_key_event_context *key_ctx);
static void	handle_move_left(t_key_event_context *key_ctx);
static void	handle_move_right(t_key_event_context *key_ctx);

/**
 * @brief Unified key handler for normal keys (callback version)
 * @param key Pressed key code
 * @param key_ctx Minimal key event context (without data access)
 * 
 * Uses function pointer array stored as static local variable
 * to avoid global variable warning.
 * Handles only normal keys (movement and rotation).
 * ESC key is handled by dedicated hook.
 */
void	handle_key_event(int key, t_key_event_context *key_ctx)
{
	static const t_key_mapping	key_mappings[] = {
	{KEY_W, handle_move_forward, "Move Forward"},
	{KEY_S, handle_move_backward, "Move Backward"},
	{KEY_A, handle_move_left, "Move Left"},
	{KEY_D, handle_move_right, "Move Right"},
	{KEY_LEFT, handle_rotate_left, "Rotate Left"},
	{KEY_RIGHT, handle_rotate_right, "Rotate Right"},
	{0, NULL, NULL}
	};
	int							i;

	if (!key_ctx)
		return ;
	i = 0;
	while (key_mappings[i].handler != NULL)
	{
		if (key_mappings[i].key_code == key)
		{
			key_mappings[i].handler(key_ctx);
			return ;
		}
		i++;
	}
}

/**
 * @brief Handler for forward movement (W key)
 * @param key_ctx Key event context containing player and map context
 */
static void	handle_move_forward(t_key_event_context *key_ctx)
{
	move_player(key_ctx->player, key_ctx->map_ctx, key_ctx->player->angle);
}

/**
 * @brief Handler for backward movement (S key)
 * @param key_ctx Key event context containing player and map context
 */
static void	handle_move_backward(t_key_event_context *key_ctx)
{
	move_player(key_ctx->player, key_ctx->map_ctx, \
		key_ctx->player->angle + PI);
}

/**
 * @brief Handler for leftward movement (A key)
 * @param key_ctx Key event context containing player and map context
 */
static void	handle_move_left(t_key_event_context *key_ctx)
{
	move_player(key_ctx->player, key_ctx->map_ctx,
		key_ctx->player->angle - 1.0 / 2.0 * PI);
}

/**
 * @brief Handler for rightward movement (D key)
 * @param key_ctx Key event context containing player and map context
 */
static void	handle_move_right(t_key_event_context *key_ctx)
{
	move_player(key_ctx->player, key_ctx->map_ctx,
		key_ctx->player->angle + 1.0 / 2.0 * PI);
}
