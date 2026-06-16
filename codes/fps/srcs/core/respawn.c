#include <math.h>

#include "core/core.h"
#include "core/respawn.h"
#include "tuning.h"

/* ************************************************************************** */
void
	save_spawn(t_game* game);
void
	respawn_player(t_game* game);
int
	is_player_dead(t_game* game);
void
	update_death(t_game* game, double delta_time);
int
	check_enemy_contact(t_game* game);
static void
	kill_player(t_game* game);

/* ************************************************************************** */
// 初期化直後のカメラ状態（位置・視線・平面）をスポーン地点として保存する
void
	save_spawn(t_game* game)
{
	copy_pos(&game->spawn.pos, &game->camera.pos);
	copy_pos(&game->spawn.dir, &game->camera.dir);
	copy_pos(&game->spawn.x_dir, &game->camera.x_dir);
	copy_pos(&game->spawn.plane, &game->camera.plane);
}

/* ************************************************************************** */
// プレイヤーを初期スポーン地点へ戻す（位置と視線方向をまとめて復元する）
void
	respawn_player(t_game* game)
{
	copy_pos(&game->camera.pos, &game->spawn.pos);
	copy_pos(&game->camera.dir, &game->spawn.dir);
	copy_pos(&game->camera.x_dir, &game->spawn.x_dir);
	copy_pos(&game->camera.plane, &game->spawn.plane);
}

/* ************************************************************************** */
// 死亡演出中（死亡画像の表示中）かどうかを返す
int
	is_player_dead(t_game* game)
{
	return (game->death_timer > 0.0);
}

/* ************************************************************************** */
// 死亡タイマーを経過時間ぶん減算し、尽きた瞬間にスポーン地点へ復帰させる
void
	update_death(t_game* game, double delta_time)
{
	if (game->death_timer <= 0.0) {
		return ;
	}
	game->death_timer -= delta_time;
	if (game->death_timer <= 0.0) {
		game->death_timer = 0.0;
		respawn_player(game);
	}
}

/* ************************************************************************** */
// 全敵との中心間距離を調べ、接触があれば死亡状態へ移行して 1 を返す
int
	check_enemy_contact(t_game* game)
{
	t_enemy*	cur;
	double		dx;
	double		dy;

	cur = game->world.enemies;
	while (cur) {
		if (cur->state != ENEMY_STATE_DEAD) {
			dx = game->camera.pos.x - cur->sprite->pos.x;
			dy = game->camera.pos.y - cur->sprite->pos.y;
			if (hypot(dx, dy) <= RESPAWN_CONTACT_DIST) {
				kill_player(game);
				return (1);
			}
		}
		cur = cur->next;
	}
	return (0);
}

/* ************************************************************************** */
// 死亡演出を開始する（待機秒数をセット。実際の復帰は update_death が担う）
static void
	kill_player(t_game* game)
{
	game->death_timer = DEATH_DURATION;
}
