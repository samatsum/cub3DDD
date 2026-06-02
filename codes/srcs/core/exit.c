/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum  <samatsum@student.42.jp   >      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 19:25:55 by samatsum          #+#    #+#             */
/*   Updated: 2026/05/29 02:12:10 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "core/core.h"
#include <stdlib.h> /* free関数を使用するために追加 */
#include <unistd.h> /* write関数, STDOUT_FILENO用に追加 */

int exit_error(t_game *game, char const *str)
{
    int unused;

    if (str)
        unused = write(STDOUT_FILENO, str, ft_strlen(str));
    (void)unused;
    exit_game(game, EXIT_FAILURE);
    return (EXIT_FAILURE);
}

int
    exit_game(t_game *game, int code)
{
    clear_config(&game->config);
    clear_window(&game->window);
    clear_textures(&game->window, game->tex);
    clear_sprites(&game->sprites);

    /* ウィンドウとディスプレイ接続の破棄 */
    if (game->window.ptr)
    {
        mlx_destroy_display(game->window.ptr);
        free(game->window.ptr);
    }

    exit(code);
    return (code);
}

int
    clear_window(t_window *window)
{
    if (window->screen.img)
        mlx_destroy_image(window->ptr, window->screen.img);
    if (window->ptr && window->win)
        mlx_destroy_window(window->ptr, window->win);
    return (0);
}
