#include "core/core.h"
#include "core/mode_ops.h"
#include "core/respawn.h"
#include "rsp/rsp_game.h"
#include "enemy/enemy_utils.h"
#include "engine/render/render.h"

/* ************************************************************************** */
static void
	rsp_respawn(t_game* game);
t_mode_ops
	rsp_mode_ops(void);

/* ************************************************************************** */
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
	ops.can_shoot = 0;
	return (ops);
}
