# ==============================================================================
# 【インクルードガード検証チェッカー (Include Guard Validator)】
#
# [目的]
# 多重インクルードによるコンパイルエラーを防ぐため、ヘッダファイルの先頭と末尾に
# 適切なインクルードガードが設定されているかを検証する。
#
# [チェック基準]
# 1. 検査対象: includes/ 以下の .h ファイル。
# 2. 検出条件:
#    - ファイル名（例: config.h）から期待されるマクロ名（CONFIG_H）を生成。
#    - ファイルの最初の有効な命令が `#ifndef CONFIG_H` でない場合。[ERROR]
#    - 次の行が `# define CONFIG_H` でない場合。[ERROR]
#    - ファイルの末尾の有効な命令が `#endif` でない場合。[ERROR]
# ==============================================================================

import os
import sys

INC_DIR = "../includes"


def main():
    error_found = False

    for root, _, files in os.walk(INC_DIR):
        for file in files:
            if file.endswith(".h"):
                file_path = os.path.join(root, file)
                expected_guard = file.replace(".h", "_H").upper()

                with open(file_path, "r", encoding="utf-8") as f:
                    lines = [line.strip() for line in f.readlines() if line.strip()]

                if not lines:
                    continue

                # 42header等をスキップし、最初のプリプロセッサ命令を探す
                first_code_idx = 0
                for i, line in enumerate(lines):
                    if not line.startswith("/*") and not line.startswith("//"):
                        first_code_idx = i
                        break

                if len(lines) < first_code_idx + 2:
                    print(f"[ERROR] Header {file_path} is too short or empty.")
                    error_found = True
                    continue

                ifndef_line = lines[first_code_idx]
                define_line = lines[first_code_idx + 1]
                endif_line = lines[-1]

                if ifndef_line != f"#ifndef {expected_guard}":
                    print(
                        f"[ERROR] Missing or incorrect #ifndef in {file_path}. Expected: #ifndef {expected_guard}"
                    )
                    error_found = True

                if define_line != f"# define {expected_guard}":
                    print(
                        f"[ERROR] Missing or incorrect # define in {file_path}. Expected: # define {expected_guard}"
                    )
                    error_found = True

                if endif_line != "#endif":
                    print(f"[ERROR] Missing #endif at the end of {file_path}.")
                    error_found = True

    if not error_found:
        print("[OK] All include guards are perfectly configured.")
    else:
        sys.exit(1)


if __name__ == "__main__":
    main()
