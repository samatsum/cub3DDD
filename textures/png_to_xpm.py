import os
from PIL import Image


def convert_png_to_xpm(input_dir):
    # XPMで使用可能なアスキー文字（ダブルクォートとバックスラッシュを除外）
    safe_chars = [chr(i) for i in range(33, 127) if chr(i) not in ['"', "\\"]]

    for filename in os.listdir(input_dir):
        if not filename.lower().endswith(".png"):
            continue

        input_path = os.path.join(input_dir, filename)
        output_path = os.path.join(input_dir, os.path.splitext(filename)[0] + ".xpm")

        try:
            # 透明度（アルファチャンネル）を保持したまま RGBA で読み込む
            original_img = Image.open(input_path).convert("RGBA")
            width, height = original_img.size

            # 銃本体の黒が透過されて穴が開くのを防ぐための処理
            pixel_data = original_img.load()
            mask_data = []  # 透過すべきかどうかのフラグ

            for y in range(height):
                for x in range(width):
                    r, g, b, a = pixel_data[x, y]
                    # アルファ値が低い（透明）か、完全に黒い背景の場合は「透過」フラグを立てる
                    if a < 128 or (r == 0 and g == 0 and b == 0):
                        mask_data.append(True)
                    else:
                        mask_data.append(False)
                        # 実体部分に真っ黒(0,0,0)があるとC言語側で穴が開くため、(1,1,1)に微小シフトして保護
                        if r == 0 and g == 0 and b == 0:
                            pixel_data[x, y] = (1, 1, 1, 255)
                        else:
                            # 半透明ピクセルも実体化させるためにアルファを最大にする
                            pixel_data[x, y] = (r, g, b, 255)

            # 白浮きを防ぐため、黒背景を作成し元の画像を重ねる
            background = Image.new("RGBA", (width, height), (0, 0, 0, 255))
            blended_img = Image.alpha_composite(background, original_img)

            # XPMの色数制限に合わせて減色
            max_colors = len(safe_chars) - 1
            quantized = blended_img.convert("RGB").quantize(colors=max_colors)

            # 色パレットの作成
            palette_data = quantized.getpalette()
            color_map = {}
            for i in range(max_colors):
                idx = i * 3
                if idx + 2 < len(palette_data):
                    r, g, b = palette_data[idx : idx + 3]
                    hex_color = f"#{r:02X}{g:02X}{b:02X}"
                    color_map[i] = (safe_chars[i + 1], hex_color)

            # 透過用の文字（一番目の文字を使用）
            TRANSPARENT_CHAR = safe_chars[0]

            # XPMヘッダーの作成
            xpm_lines = []
            xpm_lines.append("/* XPM */")
            xpm_lines.append("static char * result[] = {")
            xpm_lines.append(f'"{width} {height} {len(color_map) + 1} 1",')

            # --- 修正の要 ---
            # None ではなく、明示的に完全な黒 #000000 を指定する
            xpm_lines.append(f'"{TRANSPARENT_CHAR} c #000000",')

            # その他の色定義を追加
            for index, (char, hex_color) in color_map.items():
                xpm_lines.append(f'"{char} c {hex_color}",')

            # ピクセルデータの解析と構築
            quantized_pixels = list(quantized.getdata())

            for y in range(height):
                row_chars = ""
                for x in range(width):
                    idx = y * width + x

                    if mask_data[idx]:
                        # 透過フラグが立っているピクセルは、強制的に透過用文字（#000000）にする
                        row_chars += TRANSPARENT_CHAR
                    else:
                        q_pixel = quantized_pixels[idx]
                        row_chars += color_map.get(q_pixel, (safe_chars[1], ""))[0]

                terminator = "," if y < height - 1 else ""
                xpm_lines.append(f'"{row_chars}"{terminator}')

            xpm_lines.append("};")

            # 書き出し
            with open(output_path, "w", encoding="ascii") as f:
                f.write("\n".join(xpm_lines))
            print(f"変換成功: {filename} -> {os.path.basename(output_path)}")

        except Exception as e:
            print(f"変換エラー: {filename} -> {e}")


if __name__ == "__main__":
    convert_png_to_xpm("./")
