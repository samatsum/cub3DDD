#include "core/core.h"
#include "core/mode_ops.h"
#include "core/respawn.h"
#include "enemy/enemy_utils.h"
#include "engine/render/render.h"

/* ************************************************************************** */
static int
	fps_init_noop(t_game* game);
static void
	fps_combat(t_game* game);
static void
	fps_respawn(t_game* game);
t_mode_ops
	fps_mode_ops(void);

/* ************************************************************************** */
static int
	fps_init_noop(t_game* game)
{
	(void)game;
	return (1);
}

/* ************************************************************************** */
static void
	fps_combat(t_game* game)
{
	check_enemy_contact(game);
}

/* ************************************************************************** */
static void
	fps_respawn(t_game* game)
{
	respawn_at(game, DIRECTIONS);
}

/* ************************************************************************** */
t_mode_ops
	fps_mode_ops(void)
{
	t_mode_ops	ops;

	ops.init_assets = fps_init_noop;
	ops.init_world = fps_init_noop;
	ops.combat = fps_combat;
	ops.respawn = fps_respawn;
	ops.update_enemy = update_fps_enemy;
	ops.draw_weapon = render_fps_weapon;
	ops.can_shoot = 1;
	return (ops);
}
