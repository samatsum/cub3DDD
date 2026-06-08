#ifndef ENEMY_H
# define ENEMY_H

# include "engine/render/render.h"

/* ************************************************************************** */
# define ENEMY_MAX_HP		10

# define ENEMY_STATE_IDLE	0
# define ENEMY_STATE_WALK	1
# define ENEMY_STATE_DEAD	2

// 前方宣言
struct s_game;

/* ************************************************************************** */
// 敵の実体と状態を管理する専用構造体
typedef struct s_enemy
{
	int				hp;
	int				state;
	double			dir_angle; /* 敵自身が向いている方向（ラジアン角） */
	t_sprite*		sprite;
	struct s_enemy*	next;
}	t_enemy;

/* ************************************************************************** */
t_enemy*
	add_enemy(t_enemy** enemies, t_sprite* sprite);
void
	delete_enemy(t_enemy** enemies, t_sprite** sprites, t_sprite* target);
void
	clear_enemies(t_enemy** enemies);
void
	damage_enemy(struct s_game* game, t_sprite* hit_sprite);
int
	init_enemy_textures(struct s_game* game);
void
	update_enemies(struct s_game* game, double delta_time);

#endif