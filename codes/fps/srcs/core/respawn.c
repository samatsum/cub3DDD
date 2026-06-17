#include <math.h>

#include "core/core.h"
#include "core/respawn.h"
#include "tuning.h"

/* ************************************************************************** */
void
	save_spawn(t_game* game);
void
	respawn_at(t_game* game, char const* allowed);
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
// 初期スポーンを適用する。全 N/S/E/W から1つランダムに選ぶ（1つなら必然そこ）。
// FPSの開始配置。RSPはチーム別 respawn_at で上書きする
void
	save_spawn(t_game* game)
{
	int	idx;

	idx = pick_spawn_index(&game->config, DIRECTIONS, &game->rsp_seed);
	if (idx < 0) {
		return ;
	}
	apply_spawn(&game->config, &game->camera, &game->config.spawns[idx]);
}

/* ************************************************************************** */
// allowed の向き文字に該当するスポーンから1つランダムに選んで配置する。
// FPS=DIRECTIONS（全方向）、RSP=赤 "NW" / 青 "SE"。毎回選び直す
void
	respawn_at(t_game* game, char const* allowed)
{
	int	idx;

	idx = pick_spawn_index(&game->config, allowed, &game->rsp_seed);
	if (idx < 0) {
		return ;
	}
	apply_spawn(&game->config, &game->camera, &game->config.spawns[idx]);
}

/* ************************************************************************** */
// プレイヤーを初期スポーン地点へ戻す（FPS用。全方向プールから1つ選ぶ）
void
	respawn_player(t_game* game)
{
	respawn_at(game, DIRECTIONS);
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
