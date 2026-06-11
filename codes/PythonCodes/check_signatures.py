# ==============================================================================
# 【シグネチャフォーマットチェッカー (Signature Format Checker)】
#
# [目的]
# コーディングルールの「ルール6: 戻り値の型と関数名を別々の行に分ける。
# 関数名と引数の行は先頭にタブを1つ入れる。」が守られているかを検証する。
# 未使用関数チェッカー等が正しく動作するための、フォーマットの前提条件を担保する。
#
# [チェック基準と除外条件]
# 1. 検査対象: srcs/ および includes/ ディレクトリ以下の .c / .h ファイル。
# 2. 除外対象: C言語のコメント、文字列リテラル内部、およびプリプロセッサ（#）。
# 3. 検出するルール違反:
#    - [違反A] 戻り値の型と関数名が同じ行に記述されている。
#    - [違反B] 改行はされているが、関数名行の先頭にタブ(\t)が存在しない。
#    ※ if, while, for などの制御キーワードは関数と誤認しないよう除外している。
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

    # 違反A: 行頭から[型]と[関数名]がスペースを挟んで同一行にあり、( が続くパターン
    # 例: "int main(" や "static void* update_ui("
    pattern_a = re.compile(r"^([a-zA-Z_][a-zA-Z0-9_\s\*]*?)\s+([a-zA-Z_]\w*)\s*\(")

    # 違反B: 行頭からいきなり[関数名]が始まり、( が続くパターン（タブがない）
    # 例: "main("
    pattern_b = re.compile(r"^([a-zA-Z_]\w*)\s*\(")

    # 関数名と誤認してはならないC言語のキーワード
    ignore_keywords = {"if", "while", "for", "switch", "return", "sizeof", "else"}

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
                            # マクロや空行はスキップ
                            if not line.strip() or line.lstrip().startswith("#"):
                                continue

                            # [違反A] のチェック
                            match_a = pattern_a.match(line)
                            if match_a:
                                first_word = match_a.group(1).split()[0]
                                func_name = match_a.group(2)

                                # 最初の単語や関数名が制御キーワードなら除外
                                if (
                                    first_word not in ignore_keywords
                                    and func_name not in ignore_keywords
                                ):
                                    print(
                                        f"[ERROR] Signature Format Violation (Rule 6) in {file_path} at line {line_num}"
                                    )
                                    print(
                                        f"        -> Type and function name are on the same line: '{line.strip()}'"
                                    )
                                    print(
                                        "        -> They must be separated by a newline.\n"
                                    )
                                    error_found = True
                                    continue

                            # [違反B] のチェック
                            match_b = pattern_b.match(line)
                            if match_b:
                                func_name = match_b.group(1)

                                if func_name not in ignore_keywords:
                                    print(
                                        f"[ERROR] Signature Format Violation (Rule 6) in {file_path} at line {line_num}"
                                    )
                                    print(
                                        f"        -> Function name lacks a leading tab: '{line.strip()}'"
                                    )
                                    print(
                                        "        -> Must start with exactly one tab '\\t'.\n"
                                    )
                                    error_found = True

    if not error_found:
        print("[OK] All function signatures are correctly formatted.")
    else:
        sys.exit(1)


if __name__ == "__main__":
    main()
