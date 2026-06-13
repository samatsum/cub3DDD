# DEV_DOC — 開発者向けアーキテクチャ仕様書

このドキュメントは、cub3D エンジンを保守・拡張する開発者向けの技術資料です。記述内容は **実際のソースコードと一致するもののみ** を扱っています（推測や将来構想は §7 / §8 にまとめています）。

---

## 1. 全体像

cub3D は MiniLibX（X11）上で動作する一人称 3D レンダラに、簡単なゲームロジック（収集アイテム・追跡型の敵・武器切替）を載せたものです。

レンダリングはレイキャスティング（DDA）と、画面を縦 1 列ずつ走査して壁・床・天井・スプライトを描く古典的な手法を用います。

ライフサイクルは以下のとおりです（`srcs/main.c`）。

```
main()
  └── validate_check() : 引数チェック + init_config + .cub のパース (parse_config)
  └── setup_inits()    : init_game + finish_init
  │                      （ウィンドウ生成、テクスチャ読込、スプライト/敵リスト構築、
  │                        収集数カウント、事前計算テーブル生成）
  └── setup_hooks()    : X11 イベントとループフックの登録 (mlx_hook / mlx_loop_hook)
  └── mlx_loop()       : 以後 main_loop() が毎フレーム呼ばれる
```

毎フレーム（`main_loop`, `srcs/core/loop.c`）では以下が走ります。

1. 経過時間から `time_mult` を算出（60 FPS 基準のスケール係数、上限 3.0 / `tuning.h` の `TARGET_FPS`・`MAX_TIME_MULT`）
2. キー入力状態に応じた移動・回転（`camera.c`）
3. アイテム取得判定 `check_quest`
4. 敵 AI 更新 `update_enemies`
5. `render_frame` → `update_screen` → `mlx_put_image_to_window`

## 2. ディレクトリ構成（実態）

```
cub3d/
├── Makefile
├── codes/
│   ├── includes/                      # 公開ヘッダ
│   │   ├── types.h                    # t_game ファサード、各種フラグ、入力/世界/資産/キャッシュ型
│   │   ├── tuning.h                   # コンパイル時固定の調整値・仕様定数
│   │   ├── config/
│   │   │   ├── config.h               # 設定型・マップ文字マクロ・解像度/色マクロ
│   │   │   └── defaults.h             # 速度/FOV/敵追跡秒の既定値（.cub で上書き可）
│   │   ├── core/core.h                # ライフサイクル系の公開プロトタイプ（types.h を取り込む）
│   │   ├── engine/
│   │   │   ├── input/input.h, keymap.h
│   │   │   ├── raycast/raycast.h      # t_camera, t_raysult
│   │   │   ├── render/render.h        # t_window, t_render, t_image, t_sprite
│   │   │   └── texture/texture.h
│   │   ├── enemy/enemy.h
│   │   ├── gnl/get_next_line.h
│   │   ├── ui/ui.h
│   │   └── utils/utils.h
│   │
│   └── srcs/
│       ├── main.c                     # エントリポイント
│       ├── config/                    # .cub のパースと検証
│       │   ├── config.c               # init/clear、キー対応表 g_keys[]、parse 全体の制御
│       │   ├── parse_map.c            # マップ本体 → int 配列
│       │   ├── check_map.c            # 境界・列数・文字種チェック
│       │   ├── parse_params.c         # R / F / C とスカラー(MS/RS/FOV/ET)の解析
│       │   └── parse_texture.c        # NO/SO/WE/EA/ST/FT と OI1..OI5/OP1..OP5/OC1..OC5
│       ├── core/
│       │   ├── init.c                 # finish_init / init_game / init_window / find_sprites
│       │   ├── loop.c                 # main_loop, frame_delta, 入力適用
│       │   ├── exit.c                 # 全リソース解放
│       │   ├── bmp.c                  # BMP 書き出し（フローからは未呼び出し、§7 参照）
│       │   ├── item.c                 # 収集アイテム判定 (check_quest / count_items)
│       │   └── shoot.c                # 射撃判定 (shoot_target)
│       ├── enemy/enemy.c              # 敵リストと追跡 AI（8 方向スプライト、HP 管理）
│       ├── engine/
│       │   ├── input/input.c          # X11 キーフック（WASD + 矢印 + 1/2/3 + Space + I/L/O/Esc）
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
│       │   ├── ui.c                   # ミニマップ + 収集進捗テキスト
│       │   ├── font.c                 # ビットマップ文字描画
│       │   └── crosshair.c            # クロスヘア描画
│       └── utils/                     # libft 相当の自作ユーティリティ群
│
├── codes/includes/                    # 上記ヘッダ群
├── codes/minilibx-linux/              # ベンダー: MiniLibX
├── PythonCodes/                       # clint（独自 C コーディングルール linter）と移行スクリプト
├── maps/                              # テスト用マップ
└── textures/                          # XPM アセット
```

> **注:** 旧版にあった `ui/shortcuts.c` は廃止され、文字描画は `ui/font.c` に統合されました。`t_game` の定義は `core/core.h` から `includes/types.h` へ移設済みです。

## 3. 主要な型

| 型 | 役割 | 定義場所 |
|---|---|---|
| `t_game` | すべてのサブシステムを束ねるファサード | `types.h` |
| `t_config` | 解像度・色・テクスチャパス・マップ配列・速度/FOV/敵追跡秒・`set[]` | `config/config.h` |
| `t_window` | MiniLibX のポインタ、描画用バックバッファ | `render.h` |
| `t_camera` | 位置・視線・カメラ平面・直交ベクトル | `raycast.h` |
| `t_input` | 各軸の押下状態と装備中の武器・射撃状態 | `types.h` |
| `t_world` | スプライトリストと敵リスト、収集進捗 | `types.h` |
| `t_assets` | 壁/床/天井・武器・敵のテクスチャ群 | `types.h` |
| `t_render_cache` | `camera_x[MAX_WIDTH]` / `depth[MAX_WIDTH]` / `sf_dist[MAX_HEIGHT]` | `types.h` |
| `t_timing` | フレーム制御用のタイミング情報 | `types.h` |
| `t_render` | 描画関数群に渡す軽量コンテキスト（`t_game` への依存を遮断する目的） | `render.h` |
| `t_sprite` | 距離ソート用の双リンク（`next` と `sorted`） | `render.h` |
| `t_enemy` | HP・状態・追跡タイマー・対応 `t_sprite*` | `enemy.h` |
| `t_raysult` | 1 本のレイの計算中間結果 | `raycast.h` |

### オブジェクト体系（重要）

オブジェクトは **3 カテゴリ × 最大 5 種** に拡張されています（`config/config.h`）。

- マップ文字ブロック: 通行不可 `a`〜`e`（`IMP_FIRST='a'`）、通行可 `f`〜`j`（`PAS_FIRST='f'`）、収集 `k`〜`o`（`COL_FIRST='k'`）。`OBJ_PER_CATEGORY = 5`。
- 旧文字 `2`/`3`/`4` は各ブロック 1 種目（`a`/`f`/`k`）の **別名** として後方互換で許容（`LEGACY_IMP` 等）。
- 分類は `IS_IMPASSABLE` / `IS_PASSABLE` / `IS_COLLECTIBLE`、当たり判定は `IS_BLOCKING`（`'1'` または通行不可）で行います。
- マップ文字 → テクスチャスロットは `OBJ_SLOT(c)` が連番の `t_texture_id`（`TEX_IMP_1..5` / `TEX_PAS_1..5` / `TEX_COL_1..5`）を算術で引きます。
- `.cub` の設定キーは `config.c` の `g_keys[]` で定義され、`OI1..OI5` / `OP1..OP5` / `OC1..OC5` に対応します（裸の `OI`/`OP`/`OC` は廃止）。

## 4. フレームのデータフロー

```
key_press / key_release ──► t_input
                                │
                                ▼
        main_loop ──► (入力適用) ──► t_camera 更新
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

描画オプションは `t_game.options` のフラグで制御します（`types.h`）。

| フラグ | 既定 | 切替キー |
|---|---|---|
| `FLAG_UI` | ON | `I` |
| `FLAG_SHADOWS` | ON | `L` |
| `FLAG_CROSSHAIR` | ON | `O` |
| `FLAG_SAVE` | 未使用 | — |

## 5. ビルド

```
make           # 通常ビルド（-O3 -Wall -Wextra -Werror -I codes/includes）
make clean     # オブジェクトの削除
make fclean    # 実行ファイルも含めて削除
make re        # fclean + all
```

> **既知の制約:** 現 Makefile は `-O3` 固定で `-g` を含みません。デバッガ・サニタイザを使いたい場合は `CFLAGS` を上書きしてください（§9）。

## 6. コーディング規約

- 変数宣言は関数頭、`for` 不使用、ヘッダガード必須。1 行 1 変数で型・名前をタブで縦整列。
- 命名:
  - 関数・ファイル: `動詞_名詞`（`parse_map`, `draw_wall`）
  - 構造体: `t_xxx` / `s_xxx`、列挙: `e_xxx`
  - マクロ: `UPPER_SNAKE`
- ヘッダには **マクロ定数・型定義・プロトタイプのみ**。実装は `.c` に閉じる。
- ポインタの `*` は型名側（`int* ptr;`）。`else` は `} else {`。関数定義はセパレータ + `//` の日本語コメントを伴う。
- 調整値・仕様定数は直書きせず、共有ヘッダに集約:
  - `tuning.h` … `BAREHAND_SPEED_MULT (1.3)`, `TARGET_FPS (60.0)`, `MAX_TIME_MULT (3.0)`, `BYTES_PER_PIXEL (4)` など（コンパイル時固定）。
  - `config/defaults.h` … `DEFAULT_MOVE_SPEED (0.11)`, `DEFAULT_ROTATE_SPEED (0.11)`, `DEFAULT_FOV (0.66)`, `DEFAULT_ENEMY_TRACK_SECONDS (5.0)`（`.cub` の `MS`/`RS`/`FOV`/`ET` で上書き可）。
- 層の依存（`config` ← `core` ← `engine` ← `ui`）は循環させないこと。ただし `types.h` が `config` / `raycast` / `render` / `enemy` を取り込むため、`types.h` を引くファイルは広いヘッダ集合をパースします（§7 参照）。

### 付属の lint ツール（`PythonCodes/`）

上記コーディングルールの自動検査として `clint`（Python 製）が同梱されています。

```
python3 PythonCodes/lint.py            # 全検査を実行
python3 PythonCodes/lint.py --list     # 利用可能な検査の一覧
python3 PythonCodes/lint.py --fix      # 不足セパレータ/コメント雛形を挿入
```

include 順序・ポインタ表記・制御構文の空白・`else` 形・終端改行・シグネチャ・セパレータ・重複定義・マジックナンバー・未使用関数・`static` 漏洩などを検査します。マジックナンバーの定数化移行は `apply_tier1.py` が担います。

## 7. 既知の課題と TODO

実装と乖離している点、リファクタしたい点を列挙します。新しく入ってきた人はまずここを見てください。

### (a) パーサに残るデバッグ出力

`config/config.c` の `parse_config` には、原因切り分け用の `printf("DEBUG: ...")` が複数残っています。リリース時には除去するか、`stderr` + デバッグフラグでガードすべきです。

### (b) `screenshot()` がエントリから呼ばれていない

`core/bmp.c` の `screenshot()` は実装済みですが、`main.c` / `input.c` のどこからも呼ばれていません（`FLAG_SAVE` フラグも定義のみで未使用）。さらに `screenshot()` 内で `open()` した fd が書き込みに使われず、`save_bmp()` が固定ファイル名 `screenshot.bmp` を別途開く **二重オープン** が残っています。BMP 機能を生かすなら、コマンドライン引数（例: `-save`）かキーバインドで呼び出す導線を引き、`screenshot()` のパス決定と `save_bmp()` の書き込み先を統合する必要があります。

### (c) ヘッダの include グラフが太い

`types.h` が `config.h` / `raycast.h` / `render.h` / `enemy.h` を引き、その下流もさらに引くため、`input.c` などをビルドするだけでほぼ全ヘッダをパースします。各サブモジュールのヘッダは **公開関数の前方宣言** に絞り、構造体本体はそれを必要とする `.c` でのみ展開する整理の余地があります。

### (d) `mlx_*` 関数の戻り値

`codes/minilibx-linux/` 配下の一部関数は `int` を返す宣言にもかかわらず `return` 文を欠きます。ベンダーフォークなので軽微ですが、`-Werror=return-type` を有効化すると即ビルドが落ちます。

### (e) ベンダーのバックアップファイル

`codes/minilibx-linux/` に `*.ok` のようなバックアップが残っていないか確認し、あれば `.gitignore` で除外するか削除してください。

## 8. 拡張のしどころ（任意）

- **オブジェクトのバリエーション活用**: 各カテゴリ 5 種までテクスチャを割り当てられる枠は用意済みです。マップ生成側でこれらを使い分けると表現が広がります。
- **敵テクスチャの外部指定**: 敵は内蔵の 8 方向テクスチャを用います。`.cub` 側から敵テクスチャを指定できるようにすると、複数種の敵を導入できます。
- **HUD の充実**: HP・所持武器・残弾数の表示は未実装です。`ui.c` / `font.c` を拡張して追加できます。

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

# コーディングルール検査
python3 PythonCodes/lint.py
```
