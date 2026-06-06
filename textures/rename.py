import os


def rename_upper_to_lower_png(target_dir):
    # フォルダ内のすべてのファイルを確認
    for filename in os.listdir(target_dir):
        # 大文字の .PNG で終わるファイルのみを対象とする
        if filename.endswith(".PNG"):
            old_path = os.path.join(target_dir, filename)

            # 元のファイル名から後ろの4文字（.PNG）を削り、小文字の .png を結合する
            new_filename = filename[:-4] + ".png"
            new_path = os.path.join(target_dir, new_filename)

            try:
                # ファイル名を変更
                os.rename(old_path, new_path)
                print(f"変更成功: {filename} -> {new_filename}")
            except Exception as e:
                print(f"エラー ({filename}): {e}")


if __name__ == "__main__":
    # スクリプトと同じフォルダにあるファイルを処理
    current_directory = os.path.dirname(os.path.abspath(__file__))
    rename_upper_to_lower_png(current_directory)
