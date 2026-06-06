import os


def remove_prefix_from_filenames(directory):
    # フォルダ内の全ファイルを取得
    for filename in os.listdir(directory):
        # "processed_" で始まるファイルだけを対象にする
        if filename.startswith("processed_"):
            # 新しいファイル名を作成
            new_filename = filename.replace("processed_", "", 1)

            # パスを作成
            old_path = os.path.join(directory, filename)
            new_path = os.path.join(directory, new_filename)

            # リネーム実行
            try:
                os.rename(old_path, new_path)
                print(f"リネーム成功: {filename} -> {new_filename}")
            except OSError as e:
                print(f"リネーム失敗: {filename} - {e}")


if __name__ == "__main__":
    remove_prefix_from_filenames("../")
