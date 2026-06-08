#include <stdlib.h>
#include "core/core.h"
#include "enemy/enemy.h"

/* ************************************************************************** */
t_enemy*
	add_enemy(t_enemy** enemies, t_sprite* sprite);
void
	delete_enemy(t_enemy** enemies, t_sprite** sprites, t_sprite* target);
void
	clear_enemies(t_enemy** enemies);
void
	damage_enemy(t_game* game, t_sprite* hit_sprite);

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
	new_enemy->sprite = sprite;
	new_enemy->next = *enemies;
	*enemies = new_enemy;
	return (new_enemy);
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
