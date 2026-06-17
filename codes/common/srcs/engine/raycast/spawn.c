#include <stddef.h>

#include "engine/raycast/raycast.h"
#include "config/config.h"

/* ************************************************************************** */

void
	collect_spawns(t_config* config);
void
	apply_spawn(t_config* config, t_camera* camera, t_spawn_point* sp);
int
	pick_spawn_index(t_config* config, char const* allowed, unsigned int* seed);

/* ************************************************************************** */

// マップ全体を1度走査し、N/S/E/W のセルを位置と向き文字つきで spawns[] へ全収集
// する。スポーン文字は 'A' に潰さない方針のため、起動時に1度呼べばよい
void
	collect_spawns(t_config* config)
{
	t_pos	pos;
	char	c;

	config->spawn_count = 0;
	pos.y = 0;
	while (pos.y < config->map.rows) {
		pos.x = 0;
		while (pos.x < config->map.columns) {
			c = (char)MAP_XY(pos.x, pos.y, *config);
			if (ft_in_set(c, DIRECTIONS) && config->spawn_count < MAX_SPAWNS) {
				set_pos(&config->spawns[config->spawn_count].pos, pos.x + .5, pos.y + .5);
				config->spawns[config->spawn_count].dir = c;
				config->spawn_count++;
			}
			pos.x++;
		}
		pos.y++;
	}
}

/* ************************************************************************** */

// スポーン地点1つをカメラへ適用する（位置と、向き文字に応じた視線・平面）。
// 旧 find_start_angle の向き設定を集約。マップ文字は潰さない
void
	apply_spawn(t_config* config, t_camera* camera, t_spawn_point* sp)
{
	copy_pos(&camera->pos, &sp->pos);
	if (sp->dir == 'N') {
		set_pos(&camera->dir, 0., -1.);
		set_pos(&camera->plane, config->fov, 0.);
	} else if (sp->dir == 'E') {
		set_pos(&camera->dir, 1., 0.);
		set_pos(&camera->plane, 0., config->fov);
	} else if (sp->dir == 'S') {
		set_pos(&camera->dir, 0., 1.);
		set_pos(&camera->plane, -config->fov, 0.);
	} else if (sp->dir == 'W') {
		set_pos(&camera->dir, -1., 0.);
		set_pos(&camera->plane, 0., -config->fov);
	}
	set_pos(&camera->x_dir, camera->dir.y, -camera->dir.x);
}

/* ************************************************************************** */

// spawns[] のうち allowed に含まれる向き文字の地点から1つをランダムに選び添字を
// 返す。allowed=DIRECTIONS なら全スポーンが対象（FPS）。該当なしは -1
int
	pick_spawn_index(t_config* config, char const* allowed, unsigned int* seed)
{
	int	candidates[MAX_SPAWNS];
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (i < config->spawn_count) {
		if (ft_in_set(config->spawns[i].dir, allowed)) {
			candidates[count++] = i;
		}
		i++;
	}
	if (count == 0) {
		return (-1);
	}
	return (candidates[(int)((unsigned int)rand_r(seed) % (unsigned int)count)]);
}
