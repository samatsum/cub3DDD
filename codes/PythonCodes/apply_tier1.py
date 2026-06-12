#!/usr/bin/env python3
"""apply_tier1 — Tier 1 のマジックナンバーを名前付き定数へ置換する移行スクリプト。

clint の magic 検査が挙げた中から「直すべき（調整値・仕様値・重複定数）」と精査
した箇所だけを、共有ヘッダ includes/tuning.h の #define に置き換える。

置換は makelog に出力された“行全体”を完全一致のアンカーにしているため、無関係な
数字（中央寄せの /2、BMP 行整列の 4、ASCII 比較の 9 など）には一切触れない。
見つからないアンカーは安全側に倒してスキップし、最後にまとめて報告する。

使い方:
    python3 apply_tier1.py            # 実ファイルへ適用（includes/tuning.h も生成）
    python3 apply_tier1.py --dry-run  # 変更内容のプレビューのみ（書き込まない）
    python3 apply_tier1.py --root ../ # プロジェクトルートを明示
"""

from __future__ import annotations

import argparse
import os
import sys
from pathlib import Path

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from cutils import find_project_root  # noqa: E402

# --------------------------------------------------------------------------- #
# 生成する共有ヘッダ（プロジェクトの .h コーディングルールに準拠）
# --------------------------------------------------------------------------- #

HEADER_REL = "includes/tuning.h"

HEADER_TEXT = """\
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
"""


# --------------------------------------------------------------------------- #
# 置換定義: relpath -> [(old_line, new_line), ...]
# old は makelog 由来の“行全体（前後空白除去済み）”。部分文字列として完全一致で置換。
# --------------------------------------------------------------------------- #

EDITS: dict[str, list[tuple[str, str]]] = {
    "srcs/config/config.c": [
        ("config->requested_width = 848;", "config->requested_width = WIN_MIN_WIDTH;"),
        (
            "config->requested_height = 480;",
            "config->requested_height = WIN_MIN_HEIGHT;",
        ),
        (
            "config->rotate_speed = 0.11;",
            "config->rotate_speed = DEFAULT_ROTATE_SPEED;",
        ),
        ("config->move_speed = 0.11;", "config->move_speed = DEFAULT_MOVE_SPEED;"),
        ("config->fov = 0.66;", "config->fov = DEFAULT_FOV;"),
    ],
    "srcs/config/parse_params.c": [
        ("if (tmp < 0 || tmp > 255) {", "if (tmp < 0 || tmp > RGB_MAX) {"),
    ],
    "srcs/core/loop.c": [
        ("if (*time_mult > 3.0) {", "if (*time_mult > MAX_TIME_MULT) {"),
        ("*time_mult = 3.0;", "*time_mult = MAX_TIME_MULT;"),
        ("speed_mult = 1.3;", "speed_mult = BAREHAND_SPEED_MULT;"),
    ],
    "srcs/enemy/enemy.c": [
        ("cur->track_timer = 5.0;", "cur->track_timer = ENEMY_TRACK_SECONDS;"),
        ("time_mult = delta_time * 60.0;", "time_mult = delta_time * TARGET_FPS;"),
        ("if (time_mult > 3.0) {", "if (time_mult > MAX_TIME_MULT) {"),
        ("time_mult = 3.0;", "time_mult = MAX_TIME_MULT;"),
    ],
    "srcs/core/item.c": [
        (
            "if (MAP(game->camera.pos, game->config) == '4') {",
            "if (MAP(game->camera.pos, game->config) == TILE_ITEM) {",
        ),
        (
            "if (MAP_XY(j, i, game->config) == '4') {",
            "if (MAP_XY(j, i, game->config) == TILE_ITEM) {",
        ),
    ],
    "srcs/engine/raycast/camera.c": [
        # 同一行が4箇所に出現するが replace は全件置換するため1定義でよい
        (
            "if (IN_MAP(n_pos, *config) && MAP(n_pos, *config) != '1' && "
            "MAP(n_pos, *config) != '2') {",
            "if (IN_MAP(n_pos, *config) && MAP(n_pos, *config) != '1' && "
            "MAP(n_pos, *config) != TILE_OBSTACLE) {",
        ),
        (
            "if (MAP(c->pos, *config) != '4') {",
            "if (MAP(c->pos, *config) != TILE_ITEM) {",
        ),
    ],
    "srcs/core/init.c": [
        ("if (c >= '2' && c <= '4') {", "if (c >= TILE_OBSTACLE && c <= TILE_ITEM) {"),
        # (c - '0' - 2) は数学的に (c - '2') == (c - TILE_OBSTACLE) と等価
        (
            "tex = &game->assets.tex[TEX_SPRITE + (c - '0' - 2)];",
            "tex = &game->assets.tex[TEX_SPRITE + (c - TILE_OBSTACLE)];",
        ),
    ],
    "srcs/ui/ui.c": [
        ("if (c == '1' || c == '2') {", "if (c == '1' || c == TILE_OBSTACLE) {"),
    ],
    "srcs/engine/render/draw.c": [
        (
            "dst[(int)pos->y * (w->screen.size_line / 4) + (int)pos->x] = "
            "(unsigned int)color;",
            "dst[(int)pos->y * (w->screen.size_line / BYTES_PER_PIXEL) + "
            "(int)pos->x] = (unsigned int)color;",
        ),
    ],
    "srcs/engine/texture/color.c": [
        (
            "return (*(int*)(tex->ptr + (4 * tex->width * (int)pos->y) + "
            "(4 * (int)pos->x)));",
            "return (*(int*)(tex->ptr + (BYTES_PER_PIXEL * tex->width * "
            "(int)pos->y) + (BYTES_PER_PIXEL * (int)pos->x)));",
        ),
    ],
    "srcs/core/bmp.c": [
        (
            "color = *(int*)(w->screen.ptr + (4 * (int)w->size.x * "
            "((int)w->size.y - 1 - y)) + (4 * x));",
            "color = *(int*)(w->screen.ptr + (BYTES_PER_PIXEL * (int)w->size.x * "
            "((int)w->size.y - 1 - y)) + (BYTES_PER_PIXEL * x));",
        ),
    ],
}

INCLUDE_LINE = '#include "tuning.h"'


# --------------------------------------------------------------------------- #
# include 挿入（標準ヘッダの後、自作ヘッダ群の中に置く＝include 順序ルール準拠）
# --------------------------------------------------------------------------- #


def ensure_include(content: str) -> tuple[str, bool]:
    lines = content.split("\n")
    if any(line.strip() == INCLUDE_LINE for line in lines):
        return content, False

    last_local = -1
    last_system = -1
    for i, line in enumerate(lines):
        s = line.strip()
        if s.startswith("#include") and '"' in s:
            last_local = i
        elif s.startswith("#include") and "<" in s:
            last_system = i

    if last_local >= 0:
        insert_at = last_local + 1
    elif last_system >= 0:
        insert_at = last_system + 1
    else:
        insert_at = 0
    lines.insert(insert_at, INCLUDE_LINE)
    return "\n".join(lines), True


# --------------------------------------------------------------------------- #
# メイン
# --------------------------------------------------------------------------- #


def main(argv: list[str]) -> int:
    p = argparse.ArgumentParser(description="Tier 1 マジックナンバーの定数化を適用")
    p.add_argument("--root", type=Path, default=None)
    p.add_argument("--dry-run", action="store_true", help="書き込まずプレビューのみ")
    args = p.parse_args(argv)

    root = (args.root or find_project_root()).resolve()
    print(f"project root: {root}")
    print(f"mode        : {'DRY-RUN（書き込みなし）' if args.dry_run else '適用'}\n")

    # 1) 共有ヘッダの生成
    header_path = root / HEADER_REL
    if header_path.exists():
        print(f"= {HEADER_REL} は既に存在（上書きしません）")
    else:
        print(f"+ {HEADER_REL} を生成")
        if not args.dry_run:
            header_path.write_text(HEADER_TEXT, encoding="utf-8")

    missing: list[str] = []
    changed_files = 0

    # 2) 各ファイルへ置換と include 挿入
    for relpath, edits in EDITS.items():
        path = root / relpath
        if not path.is_file():
            missing.append(f"{relpath}（ファイルが見つからない）")
            continue

        content = path.read_text(encoding="utf-8")
        original = content
        notes: list[str] = []

        for old, new in edits:
            if old in content:
                n = content.count(old)
                content = content.replace(old, new)
                notes.append(
                    f"置換 x{n}: {old[:48]}…" if len(old) > 48 else f"置換 x{n}: {old}"
                )
            else:
                missing.append(f"{relpath}: アンカー不一致 -> {old[:60]}")

        content, added = ensure_include(content)
        if added:
            notes.append(f"include 追加: {INCLUDE_LINE}")

        if content != original:
            changed_files += 1
            print(f"* {relpath}")
            for note in notes:
                print(f"      {note}")
            if not args.dry_run:
                path.write_text(content, encoding="utf-8")

    # 3) サマリ
    print()
    print(f"変更ファイル: {changed_files}")
    if missing:
        print(f"\n要確認（{len(missing)} 件はスキップしました。手動対応してください）:")
        for m in missing:
            print(f"  - {m}")
    if args.dry_run:
        print(
            "\n[DRY-RUN] 実際の書き込みは行っていません。--dry-run を外すと適用します。"
        )
    else:
        print(
            "\n適用しました。`git diff` で差分を確認し、make / clint を再実行してください。"
        )
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
