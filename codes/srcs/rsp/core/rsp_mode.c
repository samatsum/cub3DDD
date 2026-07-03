#include "core/core.h"
#include "core/mode_ops.h"
#include "core/respawn.h"
#include "rsp/rsp_game.h"
#include "enemy/enemy_utils.h"
#include "engine/render/render.h"
#include "ui/ui.h"

/* ************************************************************************** */
static void
	rsp_respawn(t_game* game);
static void
	rsp_build_status_text(t_game* game, char* buf);
static void
	rsp_build_result_text(t_game* game, char* title, char* detail);
static void
	build_rsp_score_text(t_game* game, char* buf);
static void
	rsp_clear_text_buffer(char* buf);
t_mode_ops
	rsp_mode_ops(void);

/* ************************************************************************** */
// RSPでは所属チームに対応するスポーン地点へ復帰し、手も更新する
static void
	rsp_respawn(t_game* game)
{
	if (game->player_rsp.team == TEAM_BLUE) {
		respawn_at(game, RSP_BLUE_DIRS);
	} else {
		respawn_at(game, RSP_RED_DIRS);
	}
	game->player_rsp.hand = rsp_rehand(game->player_rsp.hand, &game->rsp_seed);
}


/* ************************************************************************** */
// RSPのHUDに表示するスコアテキストを組み立てる
static void
	rsp_build_status_text(t_game* game, char* buf)
{
	build_rsp_score_text(game, buf);
}

/* ************************************************************************** */
// RSPの勝敗タイトルと最終スコアテキストを組み立てる
static void
	rsp_build_result_text(t_game* game, char* title, char* detail)
{
	rsp_clear_text_buffer(title);
	if (game->rsp_winner == (int)game->player_rsp.team) {
		ft_write_str(title, "VICTORY", 0);
	} else {
		ft_write_str(title, "DEFEAT", 0);
	}
	build_rsp_score_text(game, detail);
}

/* ************************************************************************** */
// RSPの赤青スコアを共通フォーマットでバッファへ書き込む
static void
	build_rsp_score_text(t_game* game, char* buf)
{
	int	i;

	rsp_clear_text_buffer(buf);
	i = ft_write_str(buf, "Red ", 0);
	i = ft_write_int(buf, game->rsp_score[TEAM_RED], i);
	i = ft_write_str(buf, "/", i);
	i = ft_write_int(buf, RSP_SCORE_LIMIT, i);
	i = ft_write_str(buf, "  VS  Blue ", i);
	i = ft_write_int(buf, game->rsp_score[TEAM_BLUE], i);
	i = ft_write_str(buf, "/", i);
	ft_write_int(buf, RSP_SCORE_LIMIT, i);
}

/* ************************************************************************** */
// UI用の固定長バッファをゼロクリアする
static void
	rsp_clear_text_buffer(char* buf)
{
	int	i;

	i = 0;
	while (i < UI_BUF_SIZE) {
		buf[i++] = 0;
	}
}

/* ************************************************************************** */
// RSPモードが common へ公開する操作テーブルを構築する
t_mode_ops
	rsp_mode_ops(void)
{
	t_mode_ops	ops;

	ops.init_assets = init_hand_textures;
	ops.init_world = setup_rsp_combatants;
	ops.combat = resolve_rsp_combat;
	ops.respawn = rsp_respawn;
	ops.update_enemy = update_rsp_enemy;
	ops.draw_weapon = render_rsp_hand;
	ops.build_status_text = rsp_build_status_text;
	ops.build_result_text = rsp_build_result_text;
	ops.can_shoot = 0;
	return (ops);
}
