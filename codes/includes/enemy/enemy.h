#ifndef ENEMY_H
# define ENEMY_H

/* ************************************************************************** */
# include "engine/render/render.h"

/* ************************************************************************** */
// 経路キャッシュが保持できる最大マス数。最短経路がこれを超える場合は始点側の
// 先頭このマス数だけを保持し、敵が使い切った時点で現在地から再計算する（挙動は
// 不変で再計算頻度のみ増える）。1マスあたり t_pos(16B) を消費する点に注意
# define PATH_MAX			1024

/* ************************************************************************** */
// 敵の行動状態
typedef enum e_enemy_state
{
	ENEMY_STATE_IDLE = 0,
	ENEMY_STATE_WALK,
	ENEMY_STATE_DEAD,
	ENEMY_STATE_PATROL
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
// 敵の実体と状態を管理する専用構造体（patrol_* は巡回モードの歩行状態、
// path[] は追跡経路キャッシュ。path_idx が次に向かうマスの添字、path_goal は
// 経路計算時のプレイヤーセル。プレイヤーがセルをまたぐまで再計算しない）
typedef struct s_enemy
{
	int				hp;
	int				state;
	int				patrol_active;
	int				path_valid;
	int				path_len;
	int				path_idx;
	double			dir_angle;
	double			track_timer;
	t_pos			patrol_from;
	t_pos			patrol_target;
	t_pos			path_goal;
	t_pos			path[PATH_MAX];
	t_sprite*		sprite;
	struct s_enemy*	next;
}	t_enemy;

/* ************************************************************************** */
t_enemy*
	add_enemy(t_enemy** enemies, t_sprite* sprite, int hp);
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
