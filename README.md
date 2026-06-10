# cub3D

*本プロジェクトは、42 カリキュラムの一環として samatsum によって作成されました。*

<img align="center" src="screenshot.png" alt="Screenshot of the game" />

## 概要 (Description)

cub3D は、C 言語と MiniLibX（X11）を用いて構築された、レイキャスティング（DDA 法）ベースの一人称 3D レンダリングエンジンです。`Wolfenstein 3D` 系の擬似 3D 表現に、武器切り替え・収集アイテム・敵 AI といったゲーム要素を追加しています。

### アーキテクチャ概略

```
                        [ User Input (X11 keyboard) ]
                                    │
                                    ▼
┌───────────────────────────────────────────────────────────────────┐
│                            Main Loop (mlx)                        │
│                                                                   │
│   Input (WASD/Arrows)  ─► Camera/World update ─► Renderer         │
│       │                          │                   │            │
│       ▼                          ▼                   ▼            │
│   t_input              t_camera / t_world    t_render (screen buf)│
│                                                                   │
└───────────────────────────────────────────────┬───────────────────┘
                                                │
                                                ▼
                                    Window (X11) via MiniLibX
```

## 動作要件

- Linux（X11）
- `gcc`, `make`
- 開発ヘッダ: `xorg`, `libxext-dev`, `libbsd-dev`

Debian/Ubuntu 系での導入例:

```
sudo apt-get install gcc make xorg libxext-dev libbsd-dev
```

## ビルドと実行

```
make
./cub3D maps/valid/1.cub
```

## 操作 (Controls)

| 入力 | 動作 |
|---|---|
| `W` / `S` | 前進 / 後退 |
| `A` / `D` | 左右に平行移動（ストレイフ） |
| `←` / `→` | 視点を左右に回転 |
| `1` / `2` / `3` | 武器切り替え（ピストル / フラッシュライト / 素手） |
| `Space` | 射撃（ピストル装備時のみ） |
| `I` | UI（ミニマップ・進捗表示）の表示切替 |
| `O` | クロスヘア（照準）の表示切替 |
| `L` | 距離に応じた影付きシェーディングの切替 |
| `Esc` または ウィンドウの × | 終了 |

> **注:** ドキュメントに「Q / E でも回転できる」と記載されていた時期がありましたが、現状の実装では Q/E は割り当てられていません。回転は方向キーのみです。

## マップ仕様

詳しいマップ記述ルールは [USER_DOC.md](./USER_DOC.md) を参照してください。

## ドキュメント

- 👉 **[USER_DOC.md](./USER_DOC.md)** — プレイヤー／評価者向け。起動方法、操作、マップ記述ルール。
- 👉 **[DEV_DOC.md](./DEV_DOC.md)** — 開発者向け。モジュール構造、データフロー、既知の課題、拡張ポイント。

## 参考資料 (Resources)

- [Lode's Computer Graphics Tutorial — Raycasting](https://lodev.org/cgtutor/raycasting.html)
- [A first-person engine in 265 lines (PlayfulJS)](http://www.playfuljs.com/a-first-person-engine-in-265-lines/)
- [42Paris / minilibx-linux](https://github.com/42Paris/minilibx-linux)
- [BMP format reference](https://stackoverflow.com/questions/2654480/writing-bmp-image-in-pure-c-c-without-other-libraries)