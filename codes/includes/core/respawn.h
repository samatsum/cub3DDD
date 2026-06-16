#ifndef RESPAWN_H
# define RESPAWN_H

# include "core/core.h"

/* ************************************************************************** */
// スポーン状態の保存・復元、死亡演出（タイマー）の進行、敵との接触判定を提供する。
// スポーン状態は t_game.spawn、死亡中フラグは t_game.death_timer(>0) で表す
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

#endif
