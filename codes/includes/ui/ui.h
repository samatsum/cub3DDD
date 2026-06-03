/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 13:16:59 by samatsum          #+#    #+#             */
/*   Updated: 2026/06/03 14:32:09 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UI_H
# define UI_H

# include "engine/render/render.h"

/* ************************************************************************** */
# define MAP_TILE_SIZE			5
# define SCALE					10
# define UI_BG_X				2
# define UI_BG_Y				27
# define UI_BG_SIZE				210
# define UI_TEXT_Y				5
# define UI_BUF_SIZE			100

# define COLOR_MINIMAP_WALL		0x000000
# define COLOR_MINIMAP_BG		0x10CC10
# define COLOR_MINIMAP_EMPTY	0xFFFFFF
# define COLOR_UI_BG			0xFFFFFF
# define COLOR_UI_TEXT			0xCCCCCC
# define COLOR_UI_FONT			0x000000

# define CH_OUTER				11
# define CH_LENGTH				10
# define CH_GAP					4
# define COLOR_CROSSHAIR		0xA020F0

/* ************************************************************************** */
void
	update_ui(t_render* rnd);
void
	write_ui_text(t_window* w, int collected, int to_collect);
void
	display_crosshair(t_window* w);

#endif
