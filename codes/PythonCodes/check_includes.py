import os
import sys

SRC_DIR = "../srcs"
INC_DIR = "../includes"


def check_include_order(file_path):
    error_found = False
    found_local_include = False

    with open(file_path, "r", encoding="utf-8") as f:
        for line_num, line in enumerate(f, 1):
            line = line.strip()

            if line.startswith("#include"):
                if '"' in line:
                    # 自作ヘッダ (" ") を発見
                    found_local_include = True
                elif "<" in line and ">" in line:
                    # 標準ライブラリ (< >) を発見
                    if found_local_include:
                        # 既に自作ヘッダがインクルードされた後に標準ライブラリが来た場合はルール違反
                        print(
                            f"[ERROR] Include Order Violation in {file_path}:{line_num}"
                        )
                        print(
                            f'        -> {line} must be placed before local headers ("").'
                        )
                        error_found = True
    return error_found


def main():
    has_errors = False

    # 対象ディレクトリを結合して走査
    directories = [SRC_DIR, INC_DIR]

    for directory in directories:
        for root, _, files in os.walk(directory):
            for file in files:
                if file.endswith(".c") or file.endswith(".h"):
                    file_path = os.path.join(root, file)
                    if check_include_order(file_path):
                        has_errors = True

    if not has_errors:
        print("[OK] All include orders are correct.")
    else:
        sys.exit(1)


if __name__ == "__main__":
    main()
