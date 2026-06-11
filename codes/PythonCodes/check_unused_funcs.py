# ==============================================================================
# 【未使用関数チェッカー (Dead Code Scanner)】
#
# [目的]
# 定義されているものの、プログラムのどこからも呼び出されていない関数（デッドコード）
# を検出する。不要なコードを排除し、プロジェクトの凝集度と保守性を高く保つことを目的とする。
#
# [チェック基準と除外条件]
# 1. 検査対象: srcs/ ディレクトリ以下のすべての .c ファイル。
# 2. 関数定義の抽出基準（プロジェクトコーディングルールに完全準拠）:
#    - 戻り値と関数名が改行で分かれていること
#    - 関数名の行先頭にタブ(\t)が存在すること
#    - 引数リストの次の行に中括弧 { が存在すること
# 3. 除外対象:
#    - C言語のコメント（関数名がコメント内に偶然含まれることによる使用判定の誤作動を防ぐため）
#    - エントリポイントである main 関数（他から呼び出されずOSから実行されるため除外）
# 4. 検出条件:
#    関数定義部分の1回を除き、ソースコード全体のクリーンなテキスト（コメント除去後）内に
#    その関数名が単語として（\b）一度も出現しない場合、「未使用」と判定し警告する。
#    ※関数ポインタとしてフックに登録（mlx_hook等）されるケースも単語出現としてカウントされるため安全。
# ==============================================================================

import os
import re
import sys
from collections import defaultdict

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
    func_pattern = re.compile(
        r"^([a-zA-Z_][^\n]*)\n\t([a-zA-Z_]\w*)\s*\([\s\S]*?\)\n\{", re.MULTILINE
    )
    defined_funcs = {}

    for root, _, files in os.walk(SRC_DIR):
        for file in files:
            if file.endswith(".c"):
                file_path = os.path.join(root, file)
                with open(file_path, "r", encoding="utf-8") as f:
                    raw_content = f.read()
                    clean_content = remove_comments(raw_content)
                    for match in func_pattern.finditer(clean_content):
                        func_name = match.group(2)
                        defined_funcs[func_name] = file_path

    usage_counts = defaultdict(int)
    for root, _, files in os.walk(SRC_DIR):
        for file in files:
            if file.endswith(".c"):
                with open(os.path.join(root, file), "r", encoding="utf-8") as f:
                    raw_content = f.read()
                    clean_content = remove_comments(raw_content)
                    for func in defined_funcs:
                        usage_counts[func] += len(
                            re.findall(rf"\b{func}\b", clean_content)
                        )

    error_found = False

    for func, filepath in defined_funcs.items():
        if func == "main":
            continue

        if usage_counts[func] <= 1:
            print(f"[WARNING] Unused Function: '{func}' is never called.")
            print(f"          Defined in: {filepath}")
            error_found = True

    if not error_found:
        print("[OK] No unused dead code found.")
    else:
        sys.exit(1)


if __name__ == "__main__":
    main()
