#include <math.h>
#include <stdlib.h>
#include "core/core.h"
#include "enemy/enemy.h"
#include "../minilibx-linux/mlx.h"

/* ************************************************************************** */
#ifndef M_PI
# define M_PI 3.14159265358979323846
#endif

/* ************************************************************************** */
t_enemy*
	add_enemy(t_enemy** enemies, t_sprite* sprite);
void
	delete_enemy(t_enemy** enemies, t_sprite** sprites, t_sprite* target);
void
	clear_enemies(t_enemy** enemies);
void
	damage_enemy(t_game* game, t_sprite* hit_sprite);
int
	init_enemy_textures(t_game* game);
void
	update_enemies(t_game* game, double delta_time);

/* ************************************************************************** */
// 新しい敵を生成し、リストの先頭に追加する
t_enemy*
	add_enemy(t_enemy** enemies, t_sprite* sprite)
{
	t_enemy*	new_enemy;

	new_enemy = (t_enemy*)malloc(sizeof(*new_enemy));
	if (!new_enemy) {
		return (NULL);
	}
	new_enemy->hp = ENEMY_MAX_HP;
	new_enemy->state = ENEMY_STATE_IDLE;
	new_enemy->dir_angle = 0.0; /* デフォルトで東（0ラジアン）を向かせる */
	new_enemy->sprite = sprite;
	new_enemy->next = *enemies;
	*enemies = new_enemy;
	return (new_enemy);
}

/* ************************************************************************** */
// 8方向の敵テクスチャをメモリ上にロードする
int
	init_enemy_textures(t_game* game)
{
	char*	paths[8];
	int		i;

	paths[0] = "textures/enemy/Enemy_1.xpm";
	paths[1] = "textures/enemy/Enemy_2.xpm";
	paths[2] = "textures/enemy/Enemy_3.xpm";
	paths[3] = "textures/enemy/Enemy_4.xpm";
	paths[4] = "textures/enemy/Enemy_5.xpm";
	paths[5] = "textures/enemy/Enemy_6.xpm";
	paths[6] = "textures/enemy/Enemy_7.xpm";
	paths[7] = "textures/enemy/Enemy_8.xpm";
	i = 0;
	while (i < 8) {
		game->enemy_tex[i].path = ft_strdup(paths[i]);
		game->enemy_tex[i].tex = mlx_xpm_file_to_image(game->window.ptr, game->enemy_tex[i].path, &game->enemy_tex[i].width, &game->enemy_tex[i].height);
		if (!game->enemy_tex[i].tex) {
			return (0);
		}
		game->enemy_tex[i].ptr = mlx_get_data_addr(game->enemy_tex[i].tex, &game->enemy_tex[i].bpp, &game->enemy_tex[i].size_line, &game->enemy_tex[i].endian);
		i++;
	}
	return (1);
}

/* ************************************************************************** */
// 毎フレーム敵の角度を計算し、プレイヤーの視点に応じた8方向テクスチャを割り当てる
void
	update_enemies(t_game* game, double delta_time)
{
	t_enemy*	cur;
	double		dx;
	double		dy;
	double		diff;
	int			diff_idx;
	int			tex_idx;

	(void)delta_time; /* 将来のアニメーション・移動拡張用 */
	cur = game->enemies;
	while (cur) {
		dx = game->camera.pos.x - cur->sprite->pos.x;
		dy = game->camera.pos.y - cur->sprite->pos.y;
		diff = atan2(dy, dx) - cur->dir_angle;
		while (diff < 0.0) {
			diff += 2.0 * M_PI;
		}
		while (diff >= 2.0 * M_PI) {
			diff -= 2.0 * M_PI;
		}
		/* 相対角度を45度刻みの8セクター(0〜7)に変換し、画像ファイル名と一致させる反転処理を行う */
		diff_idx = (int)(floor((diff + (M_PI / 8.0)) / (M_PI / 4.0))) % 8;
		tex_idx = (8 - diff_idx) % 8;
		cur->sprite->tex = &game->enemy_tex[tex_idx];
		cur = cur->next;
	}
}

/* ************************************************************************** */
// 対象のスプライトを持つ敵を検索し、敵リストとスプライトリストの両方から削除する
void
	delete_enemy(t_enemy** enemies, t_sprite** sprites, t_sprite* target)
{
	t_enemy*	current;
	t_enemy*	previous;
	t_enemy*	tmp;

	current = *enemies;
	previous = NULL;
	while (current) {
		if (current->sprite == target) {
			tmp = current;
			if (!previous) {
				*enemies = tmp->next;
			} else {
				previous->next = tmp->next;
			}
			delete_sprite(sprites, &target->pos);
			free(tmp);
			return ;
		}
		previous = current;
		current = current->next;
	}
}

/* ************************************************************************** */
// 敵リストの全メモリを解放する
void
	clear_enemies(t_enemy** enemies)
{
	t_enemy*	tmp;

	while (*enemies) {
		tmp = (*enemies)->next;
		free(*enemies);
		*enemies = tmp;
	}
	*enemies = NULL;
}

/* ************************************************************************** */
// ヒットしたスプライトから敵を特定し、ダメージを与える
void
	damage_enemy(t_game* game, t_sprite* hit_sprite)
{
	t_enemy*	current;

	current = game->enemies;
	while (current) {
		if (current->sprite == hit_sprite) {
			if (current->hp > 0) {
				current->hp -= 1;
				if (current->hp <= 0) {
					delete_enemy(&game->enemies, &game->sprites, hit_sprite);
				}
			}
			return ;
		}
		current = current->next;
	}
}
