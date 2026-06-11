# ==============================================================================
# 【セパレータ＆コメントチェッカー (Separator & Comment Checker)】
#
# [目的]
# モジュールの視認性を高めるセパレータの挿入と、ドキュメンテーションコメント
# の記述漏れを防ぐ。可読性の高いコードベースを維持する。
#
# [チェック基準と除外条件]
# 1. 検査対象: srcs/ 以下の .c ファイル。
# 2. 検出条件 (.cファイル):
#    - 各関数定義の直前を上に辿ったとき、直近の有効行が `//` から始まる短い
#      コメントではない場合。[ERROR]
#    - その `//` コメントのさらに直近の有効行が、規定のセパレータ文字列
#      `/* ****... */` ではない場合。[ERROR]
# ==============================================================================

import os
import re
import sys

SRC_DIR = "../srcs"


def main():
    error_found = False
    func_pattern = re.compile(
        r"^([a-zA-Z_][^\n]*)\n\t([a-zA-Z_]\w*)\s*\([\s\S]*?\)\n\{", re.MULTILINE
    )
    separator_pattern = re.compile(r"^/\* \*{74} \*/$")

    for root, _, files in os.walk(SRC_DIR):
        for file in files:
            if file.endswith(".c"):
                file_path = os.path.join(root, file)
                with open(file_path, "r", encoding="utf-8") as f:
                    content = f.read()
                    lines = content.splitlines()

                    # 関数定義の開始行番号（0始まり）を抽出
                    for match in func_pattern.finditer(content):
                        func_name = match.group(2)
                        # マッチ位置までの改行を数えて行番号を特定
                        start_line_idx = content.count("\n", 0, match.start())

                        # 関数定義の上へ遡り、空行をスキップして直近の2つの有効行を取得
                        idx = start_line_idx - 1
                        header_lines = []
                        while idx >= 0 and len(header_lines) < 2:
                            line_str = lines[idx].strip()
                            if line_str:  # 空行でなければ追加
                                header_lines.append((idx + 1, line_str))
                            idx -= 1

                        if len(header_lines) < 2:
                            print(
                                f"[ERROR] Missing Documentation or Separator for '{func_name}' in {file_path}"
                            )
                            error_found = True
                            continue

                        comment_line_num, comment_str = header_lines[0]
                        sep_line_num, sep_str = header_lines[1]

                        if not comment_str.startswith("//"):
                            print(
                                f"[ERROR] Missing '//' comment for function '{func_name}' in {file_path} (above line {start_line_idx + 1})"
                            )
                            error_found = True

                        if not separator_pattern.match(sep_str):
                            print(
                                f"[ERROR] Missing or malformed separator '/* ****... */' for function '{func_name}' in {file_path} (above line {comment_line_num})"
                            )
                            error_found = True

    if not error_found:
        print("[OK] All functions have appropriate separators and comments.")
    else:
        sys.exit(1)


if __name__ == "__main__":
    main()
