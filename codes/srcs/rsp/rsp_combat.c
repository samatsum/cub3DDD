#include "core/core.h"
#include "core/respawn.h"
#include "rsp/rsp_game.h"
#include "tuning.h"

/* ************************************************************************** */

void
	resolve_rsp_combat(t_game* game);
static void
	rsp_home_rehand(t_game* game);
static void
	resolve_player_npc(t_game* game, t_enemy* npc);
static void
	resolve_npc_pair(t_game* game, t_enemy* a, t_enemy* b);
static void
	respawn_npc(t_game* game, t_enemy* npc);
static int
	in_contact(t_pos* a, t_pos* b);

/* ************************************************************************** */

// 毎フレーム、全異チームペア（プレイヤー↔NPC、NPC↔NPC）の接触を勝敗解決する。
// 当たり判定の物理（押し合い）は既存のままで、その上に rsp_outcome を乗せるだけ
void
	resolve_rsp_combat(t_game* game)
{
	t_enemy*	a;
	t_enemy*	b;

	rsp_home_rehand(game);
	a = game->world.enemies;
	while (a) {
		resolve_player_npc(game, a);
		b = a->next;
		while (b) {
			resolve_npc_pair(game, a, b);
			b = b->next;
		}
		a = a->next;
	}
}

/* ************************************************************************** */

// プレイヤーが自陣スポーンマス（赤=N/W、青=S/E）へ新たに踏み込んだ瞬間に、手を
// rsp_rehand で今と違う手へ変える。乗りっぱなしでは変えない（入った1回だけ）
static void
	rsp_home_rehand(t_game* game)
{
	int	c;
	int	on_home;

	c = MAP(game->camera.pos, game->config);
	on_home = ((game->player_rsp.team == TEAM_RED && IS_RED_SPAWN(c))
			|| (game->player_rsp.team == TEAM_BLUE && IS_BLUE_SPAWN(c)));
	if (on_home && !game->rsp_on_home) {
		game->player_rsp.hand = rsp_rehand(game->player_rsp.hand, &game->rsp_seed);
	}
	game->rsp_on_home = on_home;
}

/* ************************************************************************** */

// プレイヤーと NPC の異チーム接触を rsp_outcome で解決する。勝てば相手を即リス
// ポーン、負ければ自分が即リスポーン。味方・あいこは押し合いのみ（何もしない）
static void
	resolve_player_npc(t_game* game, t_enemy* npc)
{
	t_rsp_result	res;

	if (npc->rsp.team == game->player_rsp.team) {
		return ;
	}
	if (!in_contact(&game->camera.pos, &npc->sprite->pos)) {
		return ;
	}
	res = rsp_outcome(game->player_rsp.hand, npc->rsp.hand);
	if (res == RSP_WIN) {
		respawn_npc(game, npc);
	} else if (res == RSP_LOSE) {
		respawn_rsp_player(game);
	}
}

/* ************************************************************************** */

// NPC 同士の異チーム接触を rsp_outcome で解決する。負けた側を即リスポーンさせる
static void
	resolve_npc_pair(t_game* game, t_enemy* a, t_enemy* b)
{
	t_rsp_result	res;

	if (a->rsp.team == b->rsp.team) {
		return ;
	}
	if (!in_contact(&a->sprite->pos, &b->sprite->pos)) {
		return ;
	}
	res = rsp_outcome(a->rsp.hand, b->rsp.hand);
	if (res == RSP_WIN) {
		respawn_npc(game, b);
	} else if (res == RSP_LOSE) {
		respawn_npc(game, a);
	}
}

/* ************************************************************************** */

// NPC を自チームのスポーンプール（赤=N/W、青=S/E）へ即時に移し、手を rsp_rehand
// で必ず変える。追跡経路は無効化し IDLE に戻す。チームは不変
static void
	respawn_npc(t_game* game, t_enemy* npc)
{
	char const*	allowed;
	int			idx;

	allowed = RSP_RED_DIRS;
	if (npc->rsp.team == TEAM_BLUE) {
		allowed = RSP_BLUE_DIRS;
	}
	idx = pick_spawn_index(&game->config, allowed, &game->rsp_seed);
	if (idx >= 0) {
		copy_pos(&npc->sprite->pos, &game->config.spawns[idx].pos);
		copy_pos(&npc->rsp.spawn, &game->config.spawns[idx].pos);
	}
	npc->rsp.hand = rsp_rehand(npc->rsp.hand, &game->rsp_seed);
	npc->path_valid = 0;
	npc->state = ENEMY_STATE_IDLE;
}

/* ************************************************************************** */

// 2点の中心間距離が接触しきい値以下かを返す（接触判定。FPSの接触と同じ尺度）
static int
	in_contact(t_pos* a, t_pos* b)
{
	return (dist_pos(a, b) <= RESPAWN_CONTACT_DIST);
}
