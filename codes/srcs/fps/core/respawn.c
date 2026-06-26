#include "core/core.h"
#include "core/respawn.h"
#include "tuning.h"

/* ************************************************************************** */
int
	check_enemy_contact(t_game* game);
static void
	kill_player(t_game* game);

/* ************************************************************************** */
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
static void
	kill_player(t_game* game)
{
	game->death_timer = DEATH_DURATION;
}
