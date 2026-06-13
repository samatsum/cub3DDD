#include <stddef.h>
#include <sys/time.h>
#include "core/core.h"
#include "tuning.h"

/* ************************************************************************** */
#define BASE_FPS	60.0

int
	main_loop(t_game* game);
static double
	frame_delta(t_game* game, double* time_mult);
static int
	apply_input(t_game* game, double time_mult);
static long long
	get_current_time_ms(void);

/* ************************************************************************** */
// 毎フレーム実行されるメインループ。FPS制限・入力反映・更新・描画を統括する
int
	main_loop(t_game* game)
{
	double	delta_time;
	double	time_mult;
	int		update;

	delta_time = frame_delta(game, &time_mult);
	if (delta_time < 0.0) {
		return (0);
	}
	update = apply_input(game, time_mult);
	if (game->options != game->last_options) {
		update = 1;
		game->last_options = game->options;
	}
	if (update) {
		check_quest(game);
	}
	update_enemies(game, delta_time);
	render_frame(game);
	return (1);
}

/* ************************************************************************** */
// 経過時間を算出してFPS制限を行い、60FPS基準の時間倍率を計算する（制限内は負値）
static double
	frame_delta(t_game* game, double* time_mult)
{
	long long	now;
	double		delta_time;

	now = get_current_time_ms();
	if (game->timing.last_time == 0) {
		game->timing.last_time = now;
	}
	delta_time = (double)(now - game->timing.last_time) / 1000.0;
	if (delta_time < (1.0 / TARGET_FPS)) {
		return (-1.0);
	}
	game->timing.last_time = now;
	*time_mult = delta_time / (1.0 / BASE_FPS);
	if (*time_mult > MAX_TIME_MULT) {
		*time_mult = MAX_TIME_MULT;
	}
	return (delta_time);
}

/* ************************************************************************** */
// 入力状態をカメラへ反映する（WEP_HANDS装備時は移動速度を1.3倍に補正）
static int
	apply_input(t_game* game, double time_mult)
{
	double	speed_mult;
	int		update;

	speed_mult = 1.0;
	if (game->input.current_weapon == WEP_HANDS) {
		speed_mult = BAREHAND_SPEED_MULT;
	}
	update = 0;
	if (game->input.move.x || game->input.move.y) {
		update = move_camera(&game->camera, &game->config, &game->world, (game->input.move.x) ? 0 : 1, time_mult * speed_mult);
	}
	if (game->input.x_move.x || game->input.x_move.y) {
		update = move_perp_camera(&game->camera, &game->config, &game->world, (game->input.x_move.x) ? 0 : 1, time_mult * speed_mult);
	}
	if (game->input.rotate.x || game->input.rotate.y) {
		update = rotate_camera(&game->camera, &game->config, (game->input.rotate.x) ? 0 : 1, time_mult);
	}
	return (update);
}

/* ************************************************************************** */
// 現在のシステム時刻をミリ秒単位で取得する
static long long
	get_current_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((long long)tv.tv_sec * 1000 + (long long)tv.tv_usec / 1000);
}
