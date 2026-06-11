# ==============================================================================
# 【重複・名前衝突チェッカー (Duplicate & Collision Detector)】
#
# [目的]
# プロジェクト全体でのマクロの二重定義や、関数の名前衝突（Multiple Definition）を検出する。
# コンパイル後のリンクエラーを未然に防ぎ、グローバル名前空間の汚染（Shadowing）を防止する。
#
# [チェック基準と除外条件]
# 1. 検査対象: srcs/ および includes/ ディレクトリ以下の .c / .h ファイル。
# 2. 除外対象: C言語のコメント（ブロックコメント、行コメント）
# 3. 検出条件（マクロ）:
#    - 異なるファイル間で同一名の #define が存在する場合: [ERROR] (意図せぬ上書き)
#    - 同一ファイル内で同一名の #define が存在する場合: [WARNING] (再定義のバグ温床)
# 4. 検出条件（関数）:
#    - 複数のファイルで、static が付与されていない同一名の関数（グローバル関数）が
#      定義されている場合: [ERROR] (コンパイル時に確実なリンクエラーとなる)
#    - 複数のファイルに同一名の関数が存在するが、それらが static である、あるいは
#      グローバルと static が混在している場合: [WARNING]
#      (リンクエラーにはならないが、可読性やデバッグ時の混乱を招くため通知する)
# ==============================================================================

import os
import re
import sys
from collections import defaultdict

SRC_DIR = "../srcs"
INC_DIR = "../includes"


def remove_comments(text):
    def replacer(match):
        s = match.group(0)
        if s.startswith("/*"):
            return "\n" * s.count("\n")
        else:
            return ""

    pattern = re.compile(r"/\*.*?\*/|//.*", re.DOTALL)
    return pattern.sub(replacer, text)


def main():
    macro_defs = defaultdict(list)
    func_defs = defaultdict(list)

    macro_pattern = re.compile(r"^\s*#\s*define\s+([A-Za-z_]\w*)", re.MULTILINE)
    func_pattern = re.compile(
        r"^([a-zA-Z_][^\n]*)\n\t([a-zA-Z_]\w*)\s*\([\s\S]*?\)\n\{", re.MULTILINE
    )

    directories = [SRC_DIR, INC_DIR]

    for directory in directories:
        for root, _, files in os.walk(directory):
            for file in files:
                if file.endswith(".c") or file.endswith(".h"):
                    file_path = os.path.join(root, file)
                    with open(file_path, "r", encoding="utf-8") as f:
                        raw_content = f.read()
                        clean_content = remove_comments(raw_content)

                        for match in macro_pattern.finditer(clean_content):
                            macro_name = match.group(1)
                            macro_defs[macro_name].append(file_path)

                        if file.endswith(".c"):
                            for match in func_pattern.finditer(clean_content):
                                ret_type = match.group(1).strip()
                                func_name = match.group(2)
                                is_static = "static" in ret_type.split()
                                func_defs[func_name].append((file_path, is_static))

    error_found = False

    for macro, paths in macro_defs.items():
        unique_paths = list(set(paths))
        if len(unique_paths) > 1:
            print(f"[ERROR] Duplicate Macro: '{macro}' is defined in multiple files:")
            for p in unique_paths:
                print(f"        - {p}")
            error_found = True
        elif len(paths) > 1:
            print(
                f"[WARNING] Macro Redefined: '{macro}' is defined multiple times in {paths[0]}"
            )

    for func, locs in func_defs.items():
        if len(locs) > 1:
            global_locs = [p for p, is_stat in locs if not is_stat]

            if len(global_locs) > 1:
                print(
                    f"[ERROR] Multiple Definition: Global function '{func}' is defined in:"
                )
                for p in global_locs:
                    print(f"        - {p}")
                error_found = True
            else:
                print(
                    f"[WARNING] Name Collision: Function '{func}' shares name across files:"
                )
                for p, is_stat in locs:
                    scope = "static" if is_stat else "global"
                    print(f"        - {p} ({scope})")

    if not error_found:
        print("[OK] No critical duplicate definitions found.")
    else:
        sys.exit(1)


if __name__ == "__main__":
    main()
