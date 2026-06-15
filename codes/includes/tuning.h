#ifndef TUNING_H
# define TUNING_H

/* ************************************************************************** */
// ゲーム挙動のチューニング値（コンパイル時のみ。実行時の上書きは不可）
# define BAREHAND_SPEED_MULT	1.3
# define TARGET_FPS				60.0
# define MAX_TIME_MULT			3.0
// エンティティ(プレイヤー)の当たり半径。(動くものとの当たり半径)
# define PLAYER_RADIUS			0.5

// 敵の当たり半径（動くものとの当たり半径)
# define ENEMY_RADIUS			0.8

/* ************************************************************************** */
// 敵の移動速度倍率（基準 enemy_speed に対する係数）と巡回の到達判定しきい値。
// ARRIVE は1フレームの巡回移動量(enemy_speed*PATROL*MAX_TIME_MULT)より大きく取る
# define ENEMY_TRACK_SPEED_MULT		0.5
# define ENEMY_PATROL_SPEED_MULT	0.35
# define ENEMY_PATROL_ARRIVE		0.2

/* ************************************************************************** */
// フォーマット上の不変条件（仕様で固定。チューニング対象ではない）
# define BYTES_PER_PIXEL		4

/* ************************************************************************** */
// マップのタイル文字（'0'床 / '1'壁 は慣用のため未定義）
# define TILE_OBSTACLE			'2'
# define TILE_DECOR				'3'
# define TILE_ITEM				'4'

/* ************************************************************************** */
// フラッシュライト：正面コーンの半角[度]・届く距離[マス]・暗化の打消し量(1.0で全打消)
# define LIGHT_CONE_DEG		20.0
# define LIGHT_RANGE		50.0
# define LIGHT_BOOST		1.5

#endif
