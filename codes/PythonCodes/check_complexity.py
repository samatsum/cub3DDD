# ==============================================================================
# 【粒度・複雑度アナライザ (Granularity & Complexity Analyzer)】
#
# [目的]
# コードレビューにおいて「提言モード」へ移行する判断基準となる「関数の多責務
# （粒度が大きすぎる）」「循環的複雑度が高い（ネストが深すぎる）」兆候を検知する。
#
# [チェック基準]
# 1. 検査対象: srcs/ 以下の .c ファイル。
# 2. 検出条件:
#    - 【行数制限】関数内部（{ から } まで）の有効コード行数（コメントと空行を除く）
#      が 40行 を超える場合。[WARNING] -> 責務分割の提言対象。
#    - 【ネスト深度制限】関数内部でタブ(\t)によるインデントが 3段 以上
#      になっている行が存在する場合。[WARNING] -> 早期リターンの活用を促す対象。
# ==============================================================================

import os
import re
import sys

SRC_DIR = "../srcs"


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
    warning_found = False
    func_pattern = re.compile(
        r"^([a-zA-Z_][^\n]*)\n\t([a-zA-Z_]\w*)\s*\([\s\S]*?\)\n\{", re.MULTILINE
    )

    for root, _, files in os.walk(SRC_DIR):
        for file in files:
            if file.endswith(".c"):
                file_path = os.path.join(root, file)
                with open(file_path, "r", encoding="utf-8") as f:
                    raw_content = f.read()
                    clean_content = remove_comments_and_strings(raw_content)

                    for match in func_pattern.finditer(clean_content):
                        func_name = match.group(2)
                        start_idx = match.end() - 1  # '{' の位置

                        # 関数の終了 '}' を見つけるための括弧対応カウント
                        brace_count = 0
                        end_idx = -1
                        for i in range(start_idx, len(clean_content)):
                            if clean_content[i] == "{":
                                brace_count += 1
                            elif clean_content[i] == "}":
                                brace_count -= 1
                                if brace_count == 0:
                                    end_idx = i
                                    break

                        if end_idx == -1:
                            continue  # 解析エラー（括弧が閉じていない等）

                        # 関数ボディの抽出
                        func_body = clean_content[start_idx + 1 : end_idx]
                        lines = func_body.splitlines()

                        # 有効行のカウント
                        valid_lines = [line for line in lines if line.strip()]
                        if len(valid_lines) > 40:
                            print(
                                f"[WARNING] Function Granularity: '{func_name}' in {file_path} is {len(valid_lines)} lines long. (Threshold: 40)"
                            )
                            warning_found = True

                        # 最大ネスト深度の計測
                        # 行頭のタブ '\t' の数を計測。1段インデントされた状態がベースとなるため、\t\t\t (3段) 以上を警告
                        for line_num, line in enumerate(valid_lines, 1):
                            tab_count = len(line) - len(line.lstrip("\t"))
                            if tab_count >= 3:
                                print(
                                    f"[WARNING] Cyclomatic Complexity: High nesting depth ({tab_count} tabs) detected in '{func_name}', {file_path}."
                                )
                                print(
                                    "          -> Consider using early returns (Guard Clauses) to reduce nesting."
                                )
                                warning_found = True
                                break  # 関数あたり1回の警告で十分

    if not warning_found:
        print("[OK] All functions maintain appropriate granularity and complexity.")
    else:
        # これは提言レベルのWARNINGであるため、ビルド自体は止めない(正常終了)運用も可能ですが、
        # アナライザとしての厳密性を持たせるため異常終了とします。
        sys.exit(1)


if __name__ == "__main__":
    main()
