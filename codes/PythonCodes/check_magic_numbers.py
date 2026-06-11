# ==============================================================================
# 【マジックナンバー検出チェッカー (Magic Number Detector)】
#
# [目的]
# 可読性（Readability）および保守性を著しく低下させる「マジックナンバー（直書きの数値）」
# を検知する。定数はマクロ（#define）として適切に定義し、コードに意味を持たせることを
# 強制するための静的解析ツールである。
#
# [チェック基準と除外条件]
# 1. 検査対象: srcs/ ディレクトリ以下のすべての .c ファイル。
# 2. 除外対象領域（誤検知防止）:
#    - C言語のコメント（ブロックコメント /* ... */ および 行コメント // ...）
#    - プリプロセッサディレクティブ行（# から始まる行、例: #include, #define）
#    - 文字列リテラル内部（"..." の中身）
# 3. 許容する数値（C言語の慣習として直書きが一般的なもの）:
#    - 0  (初期化、NULL/NUL文字、成功の戻り値)
#    - 1  (インクリメント、真偽値のTrue)
#    - -1 (エラーの戻り値、無効なインデックス)
# 4. 検出条件:
#    上記を除外した上で、単語の境界（\b）に囲まれた独立した整数（負の符号も含む）が
#    コードロジック内に存在した場合に警告（[WARNING]）を出力し、異常終了する。
# ==============================================================================

import os
import re
import sys

SRC_DIR = "../srcs"


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
    number_pattern = re.compile(r"\b(-?(?!0\b|1\b|-1\b)\d+)\b")
    error_found = False

    for root, _, files in os.walk(SRC_DIR):
        for file in files:
            if file.endswith(".c"):
                file_path = os.path.join(root, file)
                with open(file_path, "r", encoding="utf-8") as f:
                    raw_content = f.read()
                    clean_content = remove_comments(raw_content)

                    for line_num, line in enumerate(clean_content.splitlines(), 1):
                        stripped_line = line.strip()

                        if not stripped_line or stripped_line.startswith("#"):
                            continue

                        code_without_strings = re.sub(r'".*?"', '""', stripped_line)
                        matches = number_pattern.findall(code_without_strings)

                        if matches:
                            print(
                                f"[WARNING] Magic Number detected in {file_path} at line {line_num}"
                            )
                            print(f"          Code: {stripped_line}")
                            print(f"          Numbers found: {', '.join(matches)}\n")
                            error_found = True

    if not error_found:
        print("[OK] No magic numbers found. Well structured!")
    else:
        sys.exit(1)


if __name__ == "__main__":
    main()
