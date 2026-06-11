# ==============================================================================
# 【コーディングスタイルチェッカー (Format Style Checker)】
#
# [目的]
# プロジェクトの指定フォーマット（コーディングルール）への準拠を自動検証する。
# 可読性を統一し、複数人開発におけるコードベースの美観を維持する。
#
# [チェック基準と除外条件]
# 1. 検査対象: srcs/ および includes/ ディレクトリ以下の .c / .h ファイル。
# 2. 除外対象: C言語のコメントや文字列リテラル内部のテキスト。
# 3. 検出するルール違反:
#    - [Rule 3] 制御構文のスペース: if, for, while の直後にスペースがなく、
#               即座に '(' が続いている場合。
#    - [Rule 7] else構文のフォーマット: else が登場する行が、
#               `} else {` または `} else if (...) {` の形を含んでいない場合。
#    - [Rule 8] ファイル終端ルール: ファイルの最後が改行文字 (\n) で
#               終わっていない場合。
# ==============================================================================

import os
import re
import sys

SRC_DIR = "../srcs"
INC_DIR = "../includes"


def remove_comments_and_strings(text):
    def replacer(match):
        s = match.group(0)
        # コメントの場合は行数を維持するために改行文字だけを残す
        if s.startswith("/*"):
            return "\n" * s.count("\n")
        elif s.startswith("//"):
            return ""
        # 文字列リテラルの場合は "" で置き換え、中身の誤検知を防ぐ
        else:
            return '""'

    # ブロックコメント、行コメント、文字列リテラルにマッチ
    pattern = re.compile(r'/\*.*?\*/|//.*|".*?"', re.DOTALL)
    return pattern.sub(replacer, text)


def main():
    error_found = False

    # 制御構文の直後に '(' が来ているパターン (スペースなし)
    missing_space_pattern = re.compile(r"\b(if|for|while)\(")

    directories = [SRC_DIR, INC_DIR]

    for directory in directories:
        for root, _, files in os.walk(directory):
            for file in files:
                if file.endswith(".c") or file.endswith(".h"):
                    file_path = os.path.join(root, file)

                    with open(file_path, "r", encoding="utf-8") as f:
                        raw_content = f.read()

                    # [Rule 8] ファイル末尾の改行チェック (生ファイルに対して実行)
                    if len(raw_content) > 0 and not raw_content.endswith("\n"):
                        print(f"[ERROR] Style Violation in {file_path}")
                        print(
                            "        -> File does not end with a newline character.\n"
                        )
                        error_found = True

                    clean_content = remove_comments_and_strings(raw_content)

                    for line_num, line in enumerate(clean_content.splitlines(), 1):
                        stripped_line = line.strip()
                        if not stripped_line:
                            continue

                        # [Rule 3] 制御構文のスペースチェック
                        match = missing_space_pattern.search(stripped_line)
                        if match:
                            keyword = match.group(1)
                            print(
                                f"[ERROR] Style Violation in {file_path} at line {line_num}"
                            )
                            print(
                                f"        -> Missing space after '{keyword}'. Use '{keyword} ('."
                            )
                            error_found = True

                        # [Rule 7] else構文のフォーマットチェック
                        # 単語としての else が含まれている場合を検証
                        if re.search(r"\belse\b", stripped_line):
                            # 正しいフォーマットの文字列が含まれているかチェック
                            if (
                                "} else {" not in stripped_line
                                and "} else if" not in stripped_line
                            ):
                                print(
                                    f"[ERROR] Style Violation in {file_path} at line {line_num}"
                                )
                                print(
                                    "        -> Invalid 'else' formatting. Must be '} else {' or '} else if'."
                                )
                                error_found = True

    if not error_found:
        print("[OK] Coding style is perfectly formatted.")
    else:
        sys.exit(1)


if __name__ == "__main__":
    main()
