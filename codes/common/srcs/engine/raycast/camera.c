#include <math.h>          /* cos, sin 関数用 */
#include <stddef.h>        /* NULL 用 */

#include "engine/raycast/raycast.h"
#include "config/config.h" /* MAP等のマクロ展開のため */
#include "core/collision.h"
#include "tuning.h"

/* ************************************************************************** */
int
	move_camera(t_camera* c, t_config* config, struct s_world* world, int direction, double time_mult);
int
	move_perp_camera(t_camera* c, t_config* config, struct s_world* world, int direction, double time_mult);
int
	rotate_camera(t_camera* c, t_config* config, int dir, double time_mult);
static void
	walk_axis(t_camera* c, t_config* config, struct s_world* world, t_pos mv);

/* ************************************************************************** */
// c を mv 方向(片軸ぶん。もう片方は0)へ動かす試み。動く軸だけ WALL_MARGIN 先を
// 先読みし、壁/マップ外でなく敵にも阻まれなければ確定する。壁の手前で止め食い込み防止
static void
	walk_axis(t_camera* c, t_config* config, struct s_world* world, t_pos mv)
{
	t_pos	next;
	t_pos	probe;

	copy_pos(&next, &c->pos);
	next.x += mv.x;
	next.y += mv.y;
	set_pos(&probe, next.x + WALL_MARGIN * ((mv.x > 0.0) - (mv.x < 0.0)),
		next.y + WALL_MARGIN * ((mv.y > 0.0) - (mv.y < 0.0)));
	if (IN_MAP(next, *config) && !IS_BLOCKING(MAP(next, *config))
		&& IN_MAP(probe, *config) && !IS_BLOCKING(MAP(probe, *config))
		&& !is_blocked_by_enemies(&c->pos, &next, world, NULL)) {
		copy_pos(&c->pos, &next);
	}
}

/* ************************************************************************** */
// 前後へ移動。壁・通行不可・他エンティティの判定と訪問済みマーキングを行う。
// 壁判定は WALL_MARGIN を見込み、壁の手前で止めて食い込みを防ぐ（NPCと同じ）
int
	move_camera(t_camera* c, t_config* config, struct s_world* world, int direction, double time_mult)
{
	t_pos	mv;
	double	actual_speed;

	/* 速度に時間倍率を掛けて、FPSに依存しない移動量を計算 */
	actual_speed = config->move_speed * time_mult;
	if (direction) {
		actual_speed = -actual_speed;
	}
	set_pos(&mv, c->dir.x * actual_speed, 0.0);
	walk_axis(c, config, world, mv);
	set_pos(&mv, 0.0, c->dir.y * actual_speed);
	walk_axis(c, config, world, mv);
	if (!IS_COLLECTIBLE(MAP(c->pos, *config)) && !ft_in_set(MAP(c->pos, *config), DIRECTIONS)) {
		MAP(c->pos, *config) = 'A';
	}
	return (1);
}

/* ************************************************************************** */
// 左右へ平行移動。壁・通行不可・他エンティティの判定とマーキングを行う。
// 壁判定は WALL_MARGIN を見込み、壁の手前で止めて食い込みを防ぐ（NPCと同じ）
int
	move_perp_camera(t_camera* c, t_config* config, struct s_world* world, int direction, double time_mult)
{
	t_pos	mv;
	double	actual_speed;

	/* 速度に時間倍率を掛けて、FPSに依存しない移動量を計算 */
	actual_speed = config->move_speed * time_mult;
	if (direction) {
		actual_speed = -actual_speed;
	}
	set_pos(&mv, c->x_dir.x * actual_speed, 0.0);
	walk_axis(c, config, world, mv);
	set_pos(&mv, 0.0, c->x_dir.y * actual_speed);
	walk_axis(c, config, world, mv);
	if (!IS_COLLECTIBLE(MAP(c->pos, *config)) && !ft_in_set(MAP(c->pos, *config), DIRECTIONS)) {
		MAP(c->pos, *config) = 'A';
	}
	return (1);
}

/* ************************************************************************** */
// カメラの視線を回転させ、方向ベクトルと平面ベクトルを更新する
int
	rotate_camera(t_camera* c, t_config* config, int dir, double time_mult)
{
	t_pos	old;
	double	actual_rot;
	double	cos_val;
	double	sin_val;

	/* 回転速度に時間倍率を掛け、可変FPSに対応するため毎回cos/sinを計算する */
	actual_rot = config->rotate_speed * time_mult;
	if (dir == 0) {
		actual_rot = -actual_rot;
	}
	cos_val = cos(actual_rot);
	sin_val = sin(actual_rot);
	copy_pos(&old, &c->dir);
	c->dir.x = (c->dir.x * cos_val) - (c->dir.y * sin_val);
	c->dir.y = (old.x * sin_val) + (c->dir.y * cos_val);
	copy_pos(&old, &c->plane);
	c->plane.x = (c->plane.x * cos_val) - (c->plane.y * sin_val);
	c->plane.y = (old.x * sin_val) + (c->plane.y * cos_val);
	copy_pos(&old, &c->x_dir);
	c->x_dir.x = (c->x_dir.x * cos_val) - (c->x_dir.y * sin_val);
	c->x_dir.y = (old.x * sin_val) + (c->x_dir.y * cos_val);
	return (1);
}
