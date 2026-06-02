/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum  <samatsum@student.42.jp   >      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 13:16:59 by samatsum          #+#    #+#             */
/*   Updated: 2026/05/29 00:15:55 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UI_H
# define UI_H

# include "../types.h"

/* ========================================================================== */
/* UI & Minimap Settings                                                    */
/* ========================================================================== */
# define MAP_TILE_SIZE			5
# define SCALE					10		/* ミニマップの画面端からの余白 */
# define UI_BG_X				2
# define UI_BG_Y				27
# define UI_BG_SIZE				210
# define UI_TEXT_Y				5
# define UI_BUF_SIZE			100		/* テキストバッファの最大長 */

/* ========================================================================== */
/* Color Palette                                                            */
/* ========================================================================== */
# define COLOR_MINIMAP_WALL		0x000000	/* 黒 */
# define COLOR_MINIMAP_BG		0x10CC10	/* 緑 (プレイヤー位置) */
# define COLOR_MINIMAP_EMPTY	0xFFFFFF	/* 白 (空きマス) */
# define COLOR_UI_BG			0xFFFFFF	/* 白 (UI背景) */
# define COLOR_UI_TEXT			0xCCCCCC	/* 灰色 (UIテキスト) */
# define COLOR_UI_FONT			0x000000	/* 黒 (描画フォント色) */

/* ========================================================================== */
/* Crosshair Settings                                                       */
/* ========================================================================== */
# define CH_OUTER				11		/* 描画を許可する最小画面サイズ */
# define CH_LENGTH				10		/* 線の長さ (ピクセル) */
# define CH_GAP					4		/* 中央の隙間の広さ (ピクセル) */
# define COLOR_CROSSHAIR		0xA020F0	//紫 (RGB: 0xA0, 0x20, 0xF0)
    
void			update_ui(t_render *rnd);
void			write_ui_text(t_window *w, int collected, int to_collect);
void			display_crosshair(t_window *w);

#endif