#include "core/core.h"             /* t_game 定義・自身のプロトタイプのため */
#include "../minilibx-linux/mlx.h" /* mlx_xpm_file_to_image, mlx_get_data_addr 用 */
#include "tuning.h"                /* DEATH_TEX_PATH, DOOR_TEX_PATH 用 */

/* ************************************************************************** */
void
	load_player_assets(t_game* game);

/* ************************************************************************** */
// プレイヤー視点のアセット（武器/手・死亡画面・扉）のテクスチャをまとめて読み込む。各テクスチャは
// パスを strdup してから mlx_xpm_file_to_image で読み、成功時(tex!=NULL)だけ data アドレスを取得する。
// 読み込み失敗は致命とせず、その武器/演出/扉が描画されないだけに留める（finish_init から切り出し）
void
	load_player_assets(t_game* game)
{
	int			i;
	const char*	paths[WEAPON_TEX_COUNT];

	paths[WTEX_PISTOL_IDLE] = "textures/arm/Arm_pistol_static.xpm";
	paths[WTEX_PISTOL_SHOOT] = "textures/arm/Arm_pistol_shoot.xpm";
	paths[WTEX_PISTOL_RECOIL] = "textures/arm/Arm_pistol_recoil.xpm";
	paths[WTEX_FLASHLIGHT] = "textures/arm/Arm_flashlight.xpm";
	paths[WTEX_HAND_LEFT] = "textures/arm/Arm_lefthand.xpm";
	paths[WTEX_HAND_RIGHT] = "textures/arm/Arm_righthand.xpm";
	i = 0;
	while (i < WEAPON_TEX_COUNT) {
		game->assets.weapon_tex[i].path = ft_strdup(paths[i]);
		game->assets.weapon_tex[i].tex = mlx_xpm_file_to_image(game->window.ptr,game->assets.weapon_tex[i].path, &game->assets.weapon_tex[i].width, &game->assets.weapon_tex[i].height);
		if (game->assets.weapon_tex[i].tex) {
			game->assets.weapon_tex[i].ptr = mlx_get_data_addr(game->assets.weapon_tex[i].tex, &game->assets.weapon_tex[i].bpp, &game->assets.weapon_tex[i].size_line, &game->assets.weapon_tex[i].endian);
		}
		i++;
	}
	game->assets.death_tex.path = ft_strdup(DEATH_TEX_PATH);
	game->assets.death_tex.tex = mlx_xpm_file_to_image(game->window.ptr, game->assets.death_tex.path, &game->assets.death_tex.width, &game->assets.death_tex.height);
	if (game->assets.death_tex.tex) {
		game->assets.death_tex.ptr = mlx_get_data_addr(game->assets.death_tex.tex, &game->assets.death_tex.bpp, &game->assets.death_tex.size_line, &game->assets.death_tex.endian);
	}
	game->assets.door_tex.path = ft_strdup(DOOR_TEX_PATH);
	game->assets.door_tex.tex = mlx_xpm_file_to_image(game->window.ptr, game->assets.door_tex.path, &game->assets.door_tex.width, &game->assets.door_tex.height);
	if (game->assets.door_tex.tex) {
		game->assets.door_tex.ptr = mlx_get_data_addr(game->assets.door_tex.tex,&game->assets.door_tex.bpp, &game->assets.door_tex.size_line, &game->assets.door_tex.endian);
	}
}
