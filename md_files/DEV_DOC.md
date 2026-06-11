# DEV_DOC — 開発者向けアーキテクチャ仕様書

このドキュメントは、cub3D エンジンを保守・拡張する開発者向けの技術資料です。記述内容は **実際のソースコードと一致するもののみ** を扱っています（推測や将来構想は §7 にまとめています）。

---

## 1. 全体像

cub3D は MiniLibX（X11）上で動作する一人称 3D レンダラに、簡単なゲームロジック（収集アイテム・追跡型の敵・武器切替）を載せたものです。

レンダリングはレイキャスティング（DDA）と、画面の縦軸を 1 列ずつ走査して壁・床・天井・スプライトを描く古典的な手法を用います。

ライフサイクルは以下のとおりです。

```
main()
  └── validate_check() : 引数チェック + .cub のパース
  └── setup_inits()    : ウィンドウ生成、テクスチャ読み込み、スプライト/敵リスト構築
  └── setup_hooks()    : X11 イベントとループフックの登録
  └── mlx_loop()       : 以後 main_loop() が毎フレーム呼ばれる
```

毎フレーム (`main_loop`) では以下が走ります:

1. 経過時間から `time_mult` を算出（60 FPS 基準のスケール係数、上限 3.0）
2. キー入力状態に応じて `move_camera` / `move_perp_camera` / `rotate_camera`
3. アイテム取得判定 `check_quest`
4. 敵 AI 更新 `update_enemies`
5. `render_frame` → `update_screen` → `mlx_put_image_to_window`

## 2. ディレクトリ構成（実態）

```
cub3d/
├── Makefile
├── codes/
│   ├── includes/                      # 公開ヘッダ
│   │   ├── config/config.h            # マップ・色・パスなどの設定型 / マクロ
│   │   ├── core/core.h                # t_game ファサード、各種フラグ
│   │   ├── engine/
│   │   │   ├── input/input.h, keymap.h
│   │   │   ├── raycast/raycast.h      # t_camera, t_raysult
│   │   │   ├── render/render.h        # t_window, t_render, t_sprite
│   │   │   └── texture/texture.h
│   │   ├── enemy/enemy.h
│   │   ├── gnl/get_next_line.h
│   │   ├── ui/ui.h
│   │   └── utils/utils.h
│   │
│   └── srcs/
│       ├── main.c                     # エントリポイント
│       ├── config/                    # .cub のパースと検証
│       │   ├── config.c               # init/clear と parse 全体の制御
│       │   ├── parse_map.c            # マップ本体 → int 配列
│       │   ├── check_map.c            # 境界・列数・文字種チェック
│       │   ├── parse_params.c         # R, F, C
│       │   └── parse_texture.c        # NO/SO/WE/EA/ST/FT/OI/OP/OC
│       ├── core/
│       │   ├── init.c                 # finish_init / init_window / find_sprites
│       │   ├── loop.c                 # main_loop, frame_delta, apply_input
│       │   ├── exit.c                 # 全リソース解放
│       │   ├── bmp.c                  # BMP 書き出し（フローからは未呼び出し、§7-(b) 参照）
│       │   ├── item.c                 # 収集アイテム判定
│       │   └── shoot.c                # 射撃判定
│       ├── enemy/enemy.c              # 敵リストと追跡 AI
│       ├── engine/
│       │   ├── input/input.c          # X11 キーフック
│       │   ├── raycast/
│       │   │   ├── raycast.c          # DDA 本体
│       │   │   ├── camera.c           # 移動・回転（time_mult 対応）
│       │   │   └── tables.c           # camera_x / sf_dist の事前計算
│       │   ├── render/
│       │   │   ├── screen.c           # フレーム合成 (update_screen)
│       │   │   ├── draw.c             # 低レベル描画 (pixel/line/rect)
│       │   │   ├── draw_wall.c
│       │   │   ├── draw_sky_floor.c
│       │   │   ├── draw_weapon.c      # 武器・手のオーバーレイ
│       │   │   ├── sprite.c           # スプライト描画
│       │   │   └── sprite_utils.c     # ソートとリスト操作
│       │   └── texture/
│       │       ├── texture.c          # .xpm 読み込み、透明領域判定
│       │       └── color.c            # シェーディング
│       ├── gnl/
│       │   ├── get_next_line.c
│       │   └── get_next_line_utils.c
│       ├── ui/
│       │   ├── ui.c                   # ミニマップ + テキスト
│       │   ├── crosshair.c
│       │   └── shortcuts.c            # 実体は mlx_string_put のラッパ 1 本のみ
│       └── utils/                     # libft 相当の自作ユーティリティ群
│
├── codes/minilibx-linux/              # ベンダー: MiniLibX
├── maps/                              # テスト用マップ
└── textures/                          # XPM アセット
```

## 3. 主要な型

| 型 | 役割 |
|---|---|
| `t_game` | すべてのサブシステムを束ねるファサード（`core.h`） |
| `t_config` | 解像度・色・テクスチャパス・マップ配列・各種速度 |
| `t_window` | MiniLibX のポインタ、描画用バックバッファ |
| `t_camera` | 位置 `pos`・視線 `dir`・カメラ平面 `plane`・直交ベクトル `x_dir` |
| `t_input` | 各軸の押下状態と装備中の武器 |
| `t_world` | スプライトリストと敵リスト、収集進捗 |
| `t_assets` | 壁/床/天井/スプライト/武器/敵のテクスチャ群 |
| `t_render_cache` | `camera_x[MAX_WIDTH]` / `depth[MAX_WIDTH]` / `sf_dist[MAX_HEIGHT]` |
| `t_render` | 描画関数群に渡す軽量コンテキスト（`t_game` への依存を遮断する目的） |
| `t_sprite` | 距離ソート用の双リンク（`next` と `sorted`） |
| `t_enemy` | HP・状態・追跡タイマー・対応 `t_sprite*` |
| `t_raysult` | 1 本のレイの計算中間結果（マップ座標、ステップ、wall_x など） |

## 4. フレームのデータフロー

```
key_press / key_release ──► t_input
                                │
                                ▼
        main_loop ──► apply_input ──► t_camera 更新
                  ──► check_quest  ──► t_world.collected
                  ──► update_enemies ─► t_enemy.track_timer, t_sprite.pos
                  ──► render_frame
                       └── update_screen
                            ├── 列ごとに ray_cast → cache.depth[i]
                            ├── draw_wall / draw_sky_floor
                            ├── draw_sprites (sort_sprites)
                            ├── draw_weapon
                            ├── display_crosshair (FLAG_CROSSHAIR)
                            └── update_ui       (FLAG_UI)
                       └── mlx_put_image_to_window + write_ui_text
```

## 5. ビルド

```
make           # 通常ビルド (現状 -O3 -Wall -Wextra -Werror)
make clean     # オブジェクトの削除
make fclean    # 実行ファイルも含めて削除
make re        # fclean + all
```

> **既知の問題:** 現 Makefile は `-O3` 固定で `-g` を含みません。デバッガ・サニタイザを使いたい場合は手動で `CFLAGS` を上書きしてください。`make CFLAGS="-O0 -g3 -fsanitize=address -Wall -Wextra -I codes/includes"` のように指定できます。

## 6. コーディング規約

- 変数宣言は関数頭、`for` 不使用、ヘッダガード必須。
- 命名:
  - 関数・ファイル: `動詞_名詞`（`parse_map`, `draw_wall`）
  - 構造体: `t_xxx`
  - マクロ: `UPPER_SNAKE`
- ヘッダには **マクロ定数とプロトタイプのみ**。実装は `.c` に閉じる。
- パーサ層・コア層・エンジン層・UI 層は循環依存しないこと（`#include` のグラフを参照）。

## 7. 既知の課題と TODO

実装と乖離している点、リファクタしたい点を列挙します。新しく入ってきた人はまずここを見てください。

### (b) `screenshot()` がエントリから呼ばれていない

`core/bmp.c` の `screenshot()` 関数は実装されていますが、`main.c` / `input.c` のどこからも呼び出されていません。さらに `screenshot()` 内で `open()` した fd が使われずに捨てられ、`save_bmp()` が固定ファイル名 `screenshot.bmp` を別途開いてしまうという二重オープン問題があります。BMP 機能を生かすなら、コマンドライン引数（例: `-save`）かキーバインドを通じて呼び出す導線を引き、`screenshot()` 内のファイルパス決定と `save_bmp()` の書き込み先を統合する必要があります。

### (c) GNL の `parse_config` 内でのライフタイム

`config.c` の `parse_config` は `while (get_next_line(...))` を抜けた直後にもう一度 `line` を参照しています。`get_next_line` の最終呼び出し時に `line` がどう設定されるかの契約に強く依存しており、現状の実装では **解放後参照** と **二重解放** のリスクがあります。早急に修正対象です。

### (d) ヘッダの include グラフが太い

`core.h` が `config.h` / `render.h` / `raycast.h` / `enemy.h` を引き、その下流もさらに引いていくため、`input.c` をビルドするだけでほぼ全ヘッダをパースします。サブモジュールのヘッダは **公開関数の前方宣言** に絞り、構造体の本体定義はその実装が必要な `.c` でのみ展開するよう整理する余地があります。

### (f) `mlx_*` 関数の戻り値

`codes/minilibx-linux/` 配下の `mlx_clear_window.c` などは「`int` を返す」と宣言されているにもかかわらず `return` 文がありません。ベンダーフォークなので軽い修正で済みますが、`-Werror=return-type` を有効にすると即ビルドが落ちます。

### (g) `mlx_xpm.c.ok` などの追跡漏れ

`codes/minilibx-linux/mlx_xpm.c.ok` のようなバックアップファイルがリポジトリに残っています。`.gitignore` で除外するか、削除してください。

## 8. 拡張のしどころ（任意）

- **マップ仕様の拡張**: 現状 `OP`（通行可スプライト）と敵 `M` は同じテクスチャを共有してしまうケースがあります。マップ側で敵テクスチャを指定できるようにすると、複数種の敵を導入できます。
- **HUD の充実**: HP・所持武器・残弾数の表示は未実装です。`ui.c` を拡張する形で追加できます。
- **設定ファイルへの統合**: `MOVE_SPEED` の二重管理を解消したうえで、`R`/`F`/`C` と同列に `MS`（移動速度）や `RS`（回転速度）を `.cub` から読み込めるようにすると、マップごとに挙動を変えられます。

---

## 9. 開発の始め方

```
git clone <repo>
cd cub3d
make

# 通常起動
./cub3D maps/valid/1.cub

# Address Sanitizer 付きで起動（推奨）
make fclean
make CFLAGS="-O0 -g3 -fsanitize=address -Wall -Wextra -Werror -I codes/includes"
./cub3D maps/valid/1.cub

# メモリリーク検査（MiniLibX 由来のリークは抑制対象）
valgrind --leak-check=full --suppressions=mlx.supp ./cub3D maps/valid/1.cub
```