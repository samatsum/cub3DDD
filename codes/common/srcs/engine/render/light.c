#include <stdlib.h>

#include "config/config.h"
#include "types.h"
#include "engine/render/light.h"
#include "tuning.h"

/* ************************************************************************** */
int
	build_lights(struct s_world* world, struct s_config* config);
void
	clear_lights(struct s_world* world);
double
	spotlight_factor(struct s_world* world, double wx, double wy);
int
	apply_spotlight(int color, double factor);
static int
	count_passable(struct s_config* config);
static void
	fill_lights(t_light* lights, struct s_config* config);
static void
	fill_lights(t_light* lights, struct s_config* config);
static int
	clamp_255(int v);
double
	spotlight_shade(double divide, double factor);

/* ************************************************************************** */
// 装飾スプライト(通行可)のマス中心を光源とする配列を確保する（0個なら何もしない）
int
	build_lights(struct s_world* world, struct s_config* config)
{
	int	count;

	world->lights = NULL;
	world->light_count = 0;
	count = count_passable(config);
	if (count <= 0) {
		return (1);
	}
	world->lights = (t_light*)malloc(sizeof(t_light) * count);
	if (!world->lights) {
		return (0);
	}
	world->light_count = count;
	fill_lights(world->lights, config);
	return (1);
}

/* ************************************************************************** */
// 光源配列を解放し、二重解放を防ぐためNULLと0で初期化し直す
void
	clear_lights(struct s_world* world)
{
	if (world->lights) {
		free(world->lights);
	}
	world->lights = NULL;
	world->light_count = 0;
}

/* ************************************************************************** */
// 指定ワールド座標に最も強く届く光源の照度係数[0,1]を返す（sqrt不要の二乗減衰）
double
	spotlight_factor(struct s_world* world, double wx, double wy)
{
	double	r2;
	double	d2;
	double	f;
	double	best;
	int		i;

	best = 0.0;
	r2 = SPOT_RADIUS * SPOT_RADIUS;
	i = 0;
	while (i < world->light_count) {
		d2 = (wx - world->lights[i].pos.x) * (wx - world->lights[i].pos.x)
			+ (wy - world->lights[i].pos.y) * (wy - world->lights[i].pos.y);
		if (d2 < r2) {
			f = 1.0 - d2 / r2;
			f = f * f;
			if (f > best) {
				best = f;
			}
		}
		i++;
	}
	return (best);
}

/* ************************************************************************** */
// 係数に応じて色を乗算で底上げする（0で素通り。比率保持で色相・彩度を保つ）
int
	apply_spotlight(int color, double factor)
{
	double	gain;
	int		r;
	int		g;
	int		b;

	if (factor <= 0.0) {
		return (color);
	}
	gain = 1.0 + factor * SPOT_GAIN;
	r = clamp_255((int)(((color >> 16) & 0xFF) * gain));
	g = clamp_255((int)(((color >> 8) & 0xFF) * gain));
	b = clamp_255((int)((color & 0xFF) * gain));
	return ((r << 16) | (g << 8) | b);
}

/* ************************************************************************** */
// マップ全体を走査し、通行可オブジェクトとスポーンマーカーのマス数を数える
static int
	count_passable(struct s_config* config)
{
	int	i;
	int	j;
	int	count;

	count = 0;
	i = 0;
	while (i < config->map.rows) {
		j = 0;
		while (j < config->map.columns) {
			if (IS_PASSABLE(config->map.data[i * config->map.columns + j])
				|| IS_SPAWN(config->map.data[i * config->map.columns + j])) {
				count++;
			}
			j++;
		}
		i++;
	}
	return (count);
}

/* ************************************************************************** */
// 通行可オブジェクトとスポーンマーカーのマス中心(+0.5)を各光源の座標に書き込む
static void
	fill_lights(t_light* lights, struct s_config* config)
{
	int	i;
	int	j;
	int	k;

	k = 0;
	i = 0;
	while (i < config->map.rows) {
		j = 0;
		while (j < config->map.columns) {
			if (IS_PASSABLE(config->map.data[i * config->map.columns + j])
				|| IS_SPAWN(config->map.data[i * config->map.columns + j])) {
				set_pos(&lights[k].pos, j + 0.5, i + 0.5);
				k++;
			}
			j++;
		}
		i++;
	}
}

/* ************************************************************************** */
// 1成分を0〜255に収める（乗算で255を超えた分だけ自然に白へ寄せる）
static int
	clamp_255(int v)
{
	if (v > 255) {
		return (255);
	}
	return (v);
}

/* ************************************************************************** */
// 光だまり内では距離暗化の除算係数を1.0(=フル輝度)へ引き戻し、潰れを防ぐ
double
	spotlight_shade(double divide, double factor)
{
	double	out;

	if (factor <= 0.0 || divide <= 1.0) {
		return (divide);
	}
	out = divide - (divide - 1.0) * factor;
	if (out < 1.0) {
		out = 1.0;
	}
	return (out);
}
