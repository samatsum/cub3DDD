# ==============================================================================
# 【ポインタ表記チェッカー (Pointer Notation Checker)】
#
# [目的]
# コーディングルール「共通ルール4: * は型名側に寄せる（例: int* ptr;）」の
# 準拠を検証し、開発者間のフォーマットのブレを防ぐ。
#
# [チェック基準と除外条件]
# 1. 検査対象: srcs/ および includes/ ディレクトリ以下の .c / .h ファイル。
# 2. 除外対象: C言語のコメントや文字列リテラル内部のテキスト。
# 3. 検出条件:
#    - 基本型（int, char, void等）やユーザー定義型（t_...）とアスタリスク(*)
#      の間にスペースが存在する場合。
#    - 例: `int *ptr` や `char * str` は[ERROR]とする。
#    ※乗算演算子（a * b）との誤検知を防ぐため、既知の型名のパターンに限定して検証する。
# ==============================================================================

import os
import re
import sys

SRC_DIR = "../srcs"
INC_DIR = "../includes"


def remove_comments_and_strings(text):
    def replacer(match):
        s = match.group(0)
        if s.startswith("/*"):
            return "\n" * s.count("\n")
        elif s.startswith("//"):
            return ""
        else:
            return '""'

    pattern = re.compile(r'/\*.*?\*/|//.*|".*?"', re.DOTALL)
    return pattern.sub(replacer, text)


def main():
    error_found = False

    # 型名（基本型または t_ で始まる型、struct 宣言）の直後にスペースがあり、その後に * が来るパターン
    invalid_ptr_pattern = re.compile(
        r"\b(int|char|float|double|void|long|short|unsigned|signed|size_t|t_[a-zA-Z0-9_]+)\s+\*"
    )
    invalid_struct_ptr_pattern = re.compile(r"\bstruct\s+s_[a-zA-Z0-9_]+\s+\*")

    directories = [SRC_DIR, INC_DIR]

    for directory in directories:
        for root, _, files in os.walk(directory):
            for file in files:
                if file.endswith(".c") or file.endswith(".h"):
                    file_path = os.path.join(root, file)
                    with open(file_path, "r", encoding="utf-8") as f:
                        raw_content = f.read()
                        clean_content = remove_comments_and_strings(raw_content)

                        for line_num, line in enumerate(clean_content.splitlines(), 1):
                            if invalid_ptr_pattern.search(
                                line
                            ) or invalid_struct_ptr_pattern.search(line):
                                print(
                                    f"[ERROR] Pointer Notation Violation in {file_path} at line {line_num}"
                                )
                                print(f"        -> Code: {line.strip()}")
                                print(
                                    "        -> The '*' must be attached to the type name (e.g., 'int* ptr;').\n"
                                )
                                error_found = True

    if not error_found:
        print("[OK] All pointer notations are correctly formatted.")
    else:
        sys.exit(1)


if __name__ == "__main__":
    main()
