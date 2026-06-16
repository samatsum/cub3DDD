/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* sprite_utils.c                                     :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2026/05/28 13:16:59 by samatsum          #+#    #+#             */
/* Updated: 2026/06/14 12:45:00 by samatsum         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include <math.h>
#include "engine/render/render.h" /* 補助関数の宣言 */

/* ************************************************************************** */
t_sprite*
	sort_sprites(t_camera* camera, t_sprite* sprites);
t_sprite*
	add_sorted_sprite(t_sprite** sorted, t_sprite* sprite);
t_sprite*
	add_front_sprite(t_sprite** sprites, double distance, t_pos* pos, t_tex* tex);
void
	delete_sprite(t_sprite** sprites, t_pos* pos);
void
	clear_sprites(t_sprite** sprites);

/* ************************************************************************** */
// 全てのスプライトをカメラからの距離でソートしたリストを返す（最適化版）
t_sprite*
	sort_sprites(t_camera* camera, t_sprite* sprites)
{
	t_sprite*	sorted;
	t_pos		p;

	sorted = NULL;
	copy_pos(&p, &camera->pos);
	while (sprites) {
		/* 実数の二乗和は必ず正になるため、無意味な fabs() の呼び出しを排除 */
		sprites->distance = ((p.x - sprites->pos.x) * (p.x - sprites->pos.x) + (p.y - sprites->pos.y) * (p.y - sprites->pos.y));
		sprites->sorted = NULL;
		add_sorted_sprite(&sorted, sprites);
		sprites = sprites->next;
	}
	return (sorted);
}

/* ************************************************************************** */
// 距離でソートされたリストにスプライトを挿入する
t_sprite*
	add_sorted_sprite(t_sprite** sorted, t_sprite* sprite)
{
	t_sprite*	first;
	t_sprite*	previous;

	if (!*sorted) {
		*sorted = sprite;
		return (*sorted);
	}
	first = *sorted;
	previous = NULL;
	while (*sorted && sprite->distance < (*sorted)->distance) {
		previous = *sorted;
		*sorted = (*sorted)->sorted;
	}
	if (!previous) {
		sprite->sorted = *sorted;
		*sorted = sprite;
	} else {
		sprite->sorted = previous->sorted;
		previous->sorted = sprite;
		*sorted = first;
	}
	return (sprite);
}

/* ************************************************************************** */
// リストの先頭に新しいスプライトを追加する
t_sprite*
	add_front_sprite(t_sprite** sprites, double distance, t_pos* pos, t_tex* tex)
{
	t_sprite*	new;

	new = (t_sprite*)malloc(sizeof(*new));
	if (!new) {
		return (NULL);
	}
	copy_pos(&new->pos, pos);
	new->distance = distance;
	new->next = *sprites;
	new->tex = tex;
	*sprites = new;
	return (new);
}

/* ************************************************************************** */
// 指定された位置にあるスプライトをリストから削除する
void
	delete_sprite(t_sprite** sprites, t_pos* pos)
{
	t_sprite*	tmp;
	t_sprite*	previous;
	t_sprite*	first;

	first = *sprites;
	previous = NULL;
	while (*sprites) {
		if ((int)(*sprites)->pos.x == (int)pos->x && (int)(*sprites)->pos.y == (int)pos->y) {
			tmp = *sprites;
			if (!previous) {
				*sprites = tmp->next;
			} else {
				previous->next = tmp->next;
			}
			free(tmp);
			if (previous) {
				*sprites = first;
			}
			return ;
		}
		previous = *sprites;
		*sprites = (*sprites)->next;
	}
	*sprites = first;
}

/* ************************************************************************** */
// スプライトリストの全メモリを解放する
void
	clear_sprites(t_sprite** sprites)
{
	t_sprite*	tmp;

	while (*sprites) {
		tmp = (*sprites)->next;
		free(*sprites);
		*sprites = tmp;
	}
	*sprites = NULL;
}
