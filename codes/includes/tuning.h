#ifndef TUNING_H
# define TUNING_H

/* ************************************************************************** */
// ウィンドウ解像度の下限（USER_DOC: 最小 848x480）
# define WIN_MIN_WIDTH			848
# define WIN_MIN_HEIGHT			480

/* ************************************************************************** */
// プレイヤー／カメラの初期パラメータ
# define DEFAULT_MOVE_SPEED		0.11
# define DEFAULT_ROTATE_SPEED	0.11
# define DEFAULT_FOV			0.66

/* ************************************************************************** */
// ゲーム挙動のチューニング値
# define BAREHAND_SPEED_MULT	1.3
# define ENEMY_TRACK_SECONDS	5.0
# define TARGET_FPS				60.0
# define MAX_TIME_MULT			3.0

/* ************************************************************************** */
// 入力値の上限と1ピクセルあたりのバイト数
# define RGB_MAX				255
# define BYTES_PER_PIXEL		4

/* ************************************************************************** */
// マップのタイル文字（'0'床 / '1'壁 は慣用のため未定義）
# define TILE_OBSTACLE			'2'
# define TILE_DECOR				'3'
# define TILE_ITEM				'4'

/* ************************************************************************** */

#endif
