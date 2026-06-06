import cv2
import glob
import os
import numpy as np


def process_all_pngs_strictly(folder_path):
    abs_folder_path = os.path.abspath(folder_path)
    search_path = os.path.join(abs_folder_path, "*.png")
    files = glob.glob(search_path)

    for file_path in files:
        file_name = os.path.basename(file_path)
        if file_name.startswith("processed_"):
            continue

        print(f"処理開始: {file_name}")

        # 1. 画像読み込み
        img = cv2.imread(file_path, cv2.IMREAD_UNCHANGED)
        if img is None:
            continue
        if img.shape[2] == 3:
            img = cv2.cvtColor(img, cv2.COLOR_BGR2BGRA)

        # 2. 四隅の色を取得 (BGR)
        # 厳密に判定するため、四隅の画素値を基準にする
        corners = [img[0, 0], img[0, -1], img[-1, 0], img[-1, -1]]

        # 3. 連結成分解析
        # 今回は「特定の色と同じ」場所を探すため、比較処理を行う
        gray = cv2.cvtColor(img[:, :, :3], cv2.COLOR_BGR2GRAY)

        # 閾値判定用のマスク作成（四隅の色に近いものだけを抽出）
        # ※ここでは完全一致（0,0,0）を想定するが、ノイズ対策でしきい値を設ける
        mask = cv2.inRange(img[:, :, :3], (0, 0, 0), (10, 10, 10))

        num_labels, labels = cv2.connectedComponents(mask)

        # 4. 四隅のいずれかと連結しているラベルのみを特定
        bg_labels = set()
        h, w = labels.shape
        for r, c in [(0, 0), (0, w - 1), (h - 1, 0), (h - 1, w - 1)]:
            bg_labels.add(labels[r, c])

        # 5. 背景ラベルを透過
        for label in bg_labels:
            if label == 0:
                continue
            img[labels == label, 3] = 0

        # 保存
        save_path = os.path.join(abs_folder_path, f"processed_{file_name}")
        cv2.imwrite(save_path, img)
        print(f"保存完了: {save_path}")


# 実行
process_all_pngs_strictly("../")
