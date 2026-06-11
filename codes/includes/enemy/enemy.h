#ifndef ENEMY_H
# define ENEMY_H

/* ************************************************************************** */
# include "engine/render/render.h"

/* ************************************************************************** */
// 敵1体あたりの最大HP
# define ENEMY_MAX_HP		10

/* ************************************************************************** */
// 敵の行動状態
typedef enum e_enemy_state
{
	ENEMY_STATE_IDLE = 0,
	ENEMY_STATE_WALK,
	ENEMY_STATE_DEAD
}				t_enemy_state;

// 敵テクスチャ配列の要素数（8方向ぶん。個々の要素は角度から算術的に参照される
// ため、方向名は付けず要素数のみを型付き定数として保持する）
typedef enum e_enemy_tex_id
{
	ENEMY_TEX_COUNT = 8
}				t_enemy_tex_id;

// 前方宣言
struct s_game;

/* ************************************************************************** */
// 敵の実体と状態を管理する専用構造体
typedef struct s_enemy
{
	int				hp;
	int				state;
	double			dir_angle;
	double			track_timer; /* 追跡モードの残り時間（秒）を追加 */
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
