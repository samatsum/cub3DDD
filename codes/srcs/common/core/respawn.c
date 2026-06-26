#include "core/core.h"
#include "core/respawn.h"

/* ************************************************************************** */
void
	respawn_at(t_game* game, char const* allowed);
void
	save_spawn(t_game* game);
int
	is_player_dead(t_game* game);
void
	update_death(t_game* game, double delta_time);

/* ************************************************************************** */
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
void
	save_spawn(t_game* game)
{
	respawn_at(game, DIRECTIONS);
}

/* ************************************************************************** */
int
	is_player_dead(t_game* game)
{
	return (game->death_timer > 0.0);
}

/* ************************************************************************** */
void
	update_death(t_game* game, double delta_time)
{
	if (game->death_timer <= 0.0) {
		return ;
	}
	game->death_timer -= delta_time;
	if (game->death_timer <= 0.0) {
		game->death_timer = 0.0;
		game->mode_ops.respawn(game);
	}
}
