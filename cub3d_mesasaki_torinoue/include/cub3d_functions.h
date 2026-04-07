/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_functions.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toruinoue <toruinoue@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 14:53:57 by torinoue          #+#    #+#             */
/*   Updated: 2025/11/09 01:13:22 by toruinoue        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_FUNCTIONS_H
# define CUB3D_FUNCTIONS_H

# include "cub3d_types.h"

// init_data.c
void			init_data(t_data *data, const char *filepath);

// init_resources.c
void			create_window(t_data *data);
void			init_screen_buffer(t_data *data);
void			init_player(t_player *player, t_map_context *map_ctx, \
					t_data *data);

// init_window_linux.c / init_window_mac.c
void			limit_window_size_to_screen(void *mlx_ptr, t_win *win);

// finalize_mac.c / finalize_linux.c
void			clean_exit(t_data *data, int exit_status);

// key_events.c
// int				key_press_event(int key, t_data *data);
void			move_player(t_player *player, const t_map_context *map_ctx, \
					const double move_angle);

// key_events/key_events_handlers_movement.c
void			handle_key_event(int key, t_key_event_context *key_ctx);

// key_events_hooks.c
int				key_press_unified(int key, void *param);
// int				key_press_event_callback(int key, t_data *data);

// mouse_events_hooks.c
int				mouse_press_event(int button, int x, int y, double *angle);

// window_events_hooks.c
int				close_window(t_data *data);

// scene/scene_manager.c
void			parse_scene(t_data *data);

// scene/scene_parser.c
void			parse_scene_settings(t_data *data);

// scene/parse/map_loader.c
void			load_map(t_data *data);

// scene/validation/map/elements.c
bool			is_invalid_map_elem(const char *filepath, \
					const t_map_context *map_ctx, t_data *data);

// scene/validation/settings/player_count.c
bool			is_invalid_number_map_elem(const char *filepath, t_data *data);

// scene/validation/map/closure.c
/*
** Note: This function differs from other is_invalid_* functions:
** - Other is_invalid_* functions are read-only (const parameters)
** - This function modifies map_ctx->reachable_bounds and
** - map_ctx->manhattan_distance
** - Therefore, map_ctx parameter is NOT const
*/
bool			is_invalid_map_unclosed(t_map_context *map_ctx);

// scene/scene_validation.c
void			validate_scene_settings(t_data *data);
void			validate_map_format(t_data *data);

// scene/validation/settings/texture_color.c
bool			is_invalid_texture_setting(const char *filepath, t_data *data);
bool			is_invalid_color_setting(const char *filepath, t_data *data);

// scene/validation/settings/order.c
bool			is_invalid_setting_order(const char *filepath, t_data *data);

// scene/validation/file_helper.c
bool			check_lines_with_callback(const char *filepath, \
					bool (*callback)(const char *line, void *context), \
					void *context, t_data *data);

// scene/validation/color_value.c
bool			is_invalid_color_value(const char *str);

// load_textures.c
void			load_textures(t_data *data);

// raycast/raycast.c
void			ray_casting(t_data *data);

// raycast/raycast_distance.c
double			compute_ray_distance(t_raycast *ray, \
					const t_player *player, const t_map_context *map_ctx);

// raycast/raycast_intersection.c
t_map_coord		get_h_intersection(const t_player *player, \
					const t_map_context *map_ctx, const t_raycast *ray);
t_map_coord		get_v_intersection(const t_player *player, \
					const t_map_context *map_ctx, const t_raycast *ray);

// raycast/raycast_intersection_utils.c
t_map_coord		init_h_intersection_pos(const t_player *player, \
					const t_map_context *map_ctx, const t_raycast *ray);
t_map_coord		init_v_intersection_pos(const t_player *player, \
					const t_map_context *map_ctx, const t_raycast *ray);
t_map_coord		calc_h_intersection_step(const t_map_context *map_ctx, \
					const t_raycast *ray);
t_map_coord		calc_v_intersection_step(const t_map_context *map_ctx, \
					const t_raycast *ray);

// raycast/raycast_setup.c
void			setup_ray_params(t_raycast *ray, const double ray_angle);

// collision.c
bool			is_on_wall(const t_map_context *map_ctx, \
					const t_map_coord *coord);

// graphics.c
void			draw_background(t_image *image, const t_win *win, \
					const int ceiling_color, const int floor_color);
void			render_textured_pixels(t_data *data, const int strip_id, \
					const t_image *texture, t_raycast *ray);
unsigned int	*get_pixel_ptr(const t_image *image, const int pixel_x, \
					const int pixel_y);
void			pixel_set(t_image *image, const int pixel_x, \
					const int pixel_y, const int color);

// graphics_texture.c
/*
** Note: This function takes t_data *data instead of individual members
** (data->screen and data->map_ctx) due to Norminette's limit of maximum
** 4 function parameters.
*/
void			calculate_and_render_texture_strip(t_data *data, \
					const t_image *texture, t_raycast *ray, const int strip_id);

// util/util_error.c
void			display_setting_error(t_setting_errors error, t_data *data);
void			display_error(t_defined_errors error, t_data *data);

// camera_control.c
void			view_rotation(double *angle, int direction);
void			handle_rotate_left(t_key_event_context *key_ctx);
void			handle_rotate_right(t_key_event_context *key_ctx);

// util/util_math.c
double			normalize_angle(double angle);

/*
** Calculates the Euclidean distance between two coordinates
** Formula: sqrt((x2-x1)^2 + (y2-y1)^2)
**
** Note: This function is currently unused in the codebase.
** For performance optimization, hypotenuse_unit() is used instead
** to avoid calling pow() twice (which is computationally expensive).
** The hypotenuse_unit() function computes sqrt(1 + gradient^2) which
** is more efficient for raycasting distance calculations.
**
** @param coord_a: First coordinate point
** @param coord_b: Second coordinate point
** @return: Euclidean distance between the two points
*/
double			distance(const t_map_coord *coord_a, \
					const t_map_coord *coord_b);
double			hypotenuse_unit(const double gradient);
double			radian_to_degree(const double radian);
double			safe_tan_inverse(const double tan_val);

// util/util_print.c
void			print_angle_formatted_fd(const int fd, const double radian);
void			print_double_formatted_fd(const int fd, double value);

// util/util_rgb.c
int				rgb_to_hex(const t_rgb rgb);
t_rgb			hex_to_rgb(const int hex);
int				rgb_to_hex_with_brightness(const t_rgb rgb, \
	const double brightness);

// util/util_arr.c
void			free_str_array(char ***array);
int				arrlen(char *const *arr);

// debug.c
// void			debug_put_data(t_data *data);

#endif
