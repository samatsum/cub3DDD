# ==============================================================================
# 【static 関数漏洩チェッカー (Static Scope Leakage Checker)】
#
# [目的]
# カプセル化（Information Hiding）の原則が守られているかを確認する。
# .c ファイル内で static 宣言された内部関数が、誤って .h ファイルに
# 公開プロトタイプとして記載（漏洩）していないかを検知する。
#
# [チェック基準と除外条件]
# 1. 検査対象: srcs/ 以下の .c ファイル と includes/ 以下の .h ファイル。
# 2. 除外対象: C言語のコメント（ブロックコメント、行コメント）
# 3. 抽出ルール (.c):
#    - 行頭が "static " で始まり、次行にタブと関数名が続くパターンを抽出。
# 4. 検出条件 (.h):
#    - 抽出した static 関数名が、ヘッダファイル内でプロトタイプ宣言
#      （戻り値行の次行にタブ＋関数名）として存在していた場合にエラーとする。
# ==============================================================================

import os
import re
import sys

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
    static_func_pattern = re.compile(
        r"^static\s+[^\n]+\n\t([a-zA-Z_]\w*)\s*\(", re.MULTILINE
    )
    static_funcs = {}

    for root, _, files in os.walk(SRC_DIR):
        for file in files:
            if file.endswith(".c"):
                file_path = os.path.join(root, file)
                with open(file_path, "r", encoding="utf-8") as f:
                    raw_content = f.read()
                    clean_content = remove_comments(raw_content)

                    matches = static_func_pattern.findall(clean_content)
                    for func_name in matches:
                        static_funcs[func_name] = file_path

    error_found = False

    for root, _, files in os.walk(INC_DIR):
        for file in files:
            if file.endswith(".h"):
                header_path = os.path.join(root, file)
                with open(header_path, "r", encoding="utf-8") as f:
                    raw_content = f.read()
                    clean_content = remove_comments(raw_content)

                    for func_name, src_path in static_funcs.items():
                        decl_pattern = re.compile(
                            rf"^[^\n]+\n\t{func_name}\s*\(", re.MULTILINE
                        )
                        if decl_pattern.search(clean_content):
                            print(
                                f"[ERROR] Scope Violation: static function '{func_name}'"
                            )
                            print(f"        Defined as static in: {src_path}")
                            print(f"        Leaked in header    : {header_path}\n")
                            error_found = True

    if not error_found:
        print("[OK] No static functions leaked to headers.")
    else:
        sys.exit(1)


if __name__ == "__main__":
    main()
