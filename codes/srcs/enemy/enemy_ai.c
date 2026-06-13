#include <math.h>

#include "core/core.h"
#include "enemy/enemy.h"
#include "enemy/enemy_utils.h"

/* ************************************************************************** */
void
	update_enemies(t_game* game, double delta_time);
static void
	move_enemy(t_enemy* cur, t_game* game, double delta_time);
static void
	track_player(t_enemy* cur, t_game* game, double delta_time);

/* ************************************************************************** */
// 毎フレーム検知判定を行い、追跡タイマー更新・移動・テクスチャ更新を実行する
void
	update_enemies(t_game* game, double delta_time)
{
	t_enemy*	cur;
	double		dx;
	double		dy;
	double		target_angle;

	cur = game->world.enemies;
	while (cur) {
		dx = game->camera.pos.x - cur->sprite->pos.x;
		dy = game->camera.pos.y - cur->sprite->pos.y;
		target_angle = atan2(dy, dx);
		if (enemy_sees_player(cur, game, target_angle)) {
			cur->track_timer = game->config.enemy_track_seconds;
		}
		move_enemy(cur, game, delta_time);
		update_texture(cur, game, target_angle);
		cur = cur->next;
	}
}

/* ************************************************************************** */
// 追跡タイマーに従い状態を切り替える。残時間が尽きていれば待機状態へ戻す
static void
	move_enemy(t_enemy* cur, t_game* game, double delta_time)
{
	if (cur->track_timer <= 0.0) {
		cur->state = ENEMY_STATE_IDLE;
		return ;
	}
	cur->track_timer -= delta_time;
	cur->state = ENEMY_STATE_WALK;
	track_player(cur, game, delta_time);
}

/* ************************************************************************** */
// プレイヤーの現在地へBFSで最短経路を求め、次の1マスへ向かって前進する
static void
	track_player(t_enemy* cur, t_game* game, double delta_time)
{
	t_pos	next;
	int		sx;
	int		sy;
	int		gx;
	int		gy;
	double	aim_x;
	double	aim_y;

	sx = (int)floor(cur->sprite->pos.x);
	sy = (int)floor(cur->sprite->pos.y);
	gx = (int)floor(game->camera.pos.x);
	gy = (int)floor(game->camera.pos.y);
	aim_x = game->camera.pos.x;
	aim_y = game->camera.pos.y;
	if (!(sx == gx && sy == gy) && bfs_next_step(&game->config, sx, sy, gx, gy, &next)) {
		aim_x = next.x + 0.5;
		aim_y = next.y + 0.5;
	}
	cur->dir_angle = atan2(aim_y - cur->sprite->pos.y, aim_x - cur->sprite->pos.x);
	step_enemy(cur, game, delta_time);
}
