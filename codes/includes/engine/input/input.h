/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum  <samatsum@student.42.jp   >      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 19:14:41 by samatsum          #+#    #+#             */
/*   Updated: 2026/05/28 17:48:15 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENGINE_INPUT_HOOKS_H
# define ENGINE_INPUT_HOOKS_H

/* mlxのイベントフック関数群 */
int             main_loop(t_game *game);
int             exit_hook(t_game *game);
int             expose_hook(t_game *game);
int             key_press(int keycode, t_game *game);
int             key_release(int keycode, t_game *game);

#endif