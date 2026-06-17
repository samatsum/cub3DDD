#include "config/config.h"

/* ************************************************************************** */
int
	parse_map(t_config* config, t_str* map_buffer);
int
	copy_map(t_config* config, t_str* map_buffer, int* map);
static int
	build_map_flags(t_config* config);

/* ************************************************************************** */
// バッファからマップデータを解析し、設定に格納する
int
	parse_map(t_config* config, t_str* map_buffer)
{
	int*	map;

	map = NULL;
	
	printf("DEBUG parse_map: map_buffer=%p\n", (void*)map_buffer);
	config->map.columns = check_top_bottom_borders(map_buffer);
	printf("DEBUG after TB: map_buffer=%p columns=%d\n", (void*)map_buffer, config->map.columns);
	config->map.rows = check_left_right_borders(map_buffer);
	printf("DEBUG after LR: rows=%d\n", config->map.rows);
	if (config->map.columns <= 2 || config->map.rows <= 2 || !check_valid(config, map_buffer)) {
		return (0);
	}
	map = (int*)malloc(sizeof(*map) * (config->map.rows * config->map.columns));
	if (!map) {
		return (0);
	}
	if (copy_map(config, map_buffer, map) < 1) {
		free(map);
		return (0);
	}
	config->map.data = map;
	if (!build_map_flags(config)) {
		return (0);
	}
	return (1);
}

/* ************************************************************************** */
// マップバッファの内容を配列にコピーし、カメラの数を返す
int
	copy_map(t_config* config, t_str* map_buffer, int* map)
{
	int i;
	int j;
	int line;
	int has_camera;

	i = 0;
	has_camera = 0;
	while (map_buffer) {
		j = 0;
		line = 0;
		while (map_buffer->content[j]) {
			while (map_buffer->content[j] == ' ') {
				j++;
			}
			map[(i * config->map.columns) + line++] = map_buffer->content[j];
			if (ft_in_set(map_buffer->content[j], DIRECTIONS)) {
				has_camera++;
			}
			j++;
		}
		map_buffer = map_buffer->next;
		i++;
	}
	return (has_camera);
}

/* ************************************************************************** */
// マップ本体から静的なセル属性フラグ層を構築する（現状は P ロードのみを記録）
static int
	build_map_flags(t_config* config)
{
	unsigned char*	flags;
	int				total;
	int				i;

	total = config->map.rows * config->map.columns;
	flags = (unsigned char*)malloc(sizeof(*flags) * total);
	if (!flags) {
		return (0);
	}
	i = 0;
	while (i < total) {
		if (config->map.data[i] == 'P') {
			flags[i] = CELL_PATROL;
		} else {
			flags[i] = 0;
		}
		i++;
	}
	config->map.flags = flags;
	return (1);
}
