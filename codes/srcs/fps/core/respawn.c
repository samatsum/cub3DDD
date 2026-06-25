#include "core/core.h"
#include "core/respawn.h"
#include "tuning.h"
#include "rsp/rsp_game.h"

/* ************************************************************************** */
void
	save_spawn(t_game* game);
void
	respawn_at(t_game* game, char const* allowed);
void
	respawn_player(t_game* game);
void
	respawn_rsp_player(t_game* game);
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
	respawn_at(game, DIRECTIONS);
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
// RSPのプレイヤー復帰。自チームのスポーンプール（赤=N/W、青=S/E）から1つ選んで
// 即時に配置し、手を rsp_rehand で必ず別の手へ変える。チームは不変
void
	respawn_rsp_player(t_game* game)
{
	if (game->player_rsp.team == TEAM_BLUE) {
		respawn_at(game, RSP_BLUE_DIRS);
	} else {
		respawn_at(game, RSP_RED_DIRS);
	}
	game->player_rsp.hand = rsp_rehand(game->player_rsp.hand, &game->rsp_seed);
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

	cur = game->world.enemies;
	while (cur) {
		if (cur->state != ENEMY_STATE_DEAD) {
			if (dist_pos(&game->camera.pos, &cur->sprite->pos) <= RESPAWN_CONTACT_DIST) {
				kill_player(game);
				return (1);
			}
		}
		cur = cur->next;
	}
	return (0);
}

/* ************************************************************************** */
// 死亡演出を開始する（待機秒数をセット。実際の復帰は update_death が担う）。
// FPS専用。RSPの接触は resolve_rsp_combat 側で勝敗判定して即リスポーンする
static void
	kill_player(t_game* game)
{
	game->death_timer = DEATH_DURATION;
}
