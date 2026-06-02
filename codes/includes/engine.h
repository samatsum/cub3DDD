/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum  <samatsum@student.42.jp   >      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 13:16:59 by samatsum          #+#    #+#             */
/*   Updated: 2026/05/28 21:05:22 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENGINE_H
# define ENGINE_H

/* ** 構造体とマクロの基盤 */
# include "types.h"

/* ** 分割された機能別レイヤー */
/* 修正: サブディレクトリへのパスを明記する */
# include "engine/raycast/raycast.h"
# include "engine/render/render.h"
# include "engine/texture/texture.h"
# include "ui/ui.h"

#endif