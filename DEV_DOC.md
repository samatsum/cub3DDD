# DEV_DOC — 開発者向けアーキテクチャ仕様書

このドキュメントは、cub3D エンジンを保守・拡張する開発者向けの技術資料です。記述内容は **実際のソースコードと一致するもののみ** を扱っています（推測や将来構想は §8 / §9 にまとめています）。

---

## 1. 全体像

cub3D は MiniLibX（X11）上で動作する一人称 3D レンダラに、簡単なゲームロジック（収集アイテム・**巡回／追跡する敵 AI**・武器切替）を載せたものです。

レンダリングはレイキャスティング（DDA）と、画面を縦 1 列ずつ走査して壁・床・天井・スプライトを描く古典的な手法を用います。

ライフサイクルは以下のとおりです（`srcs/main.c`）。

```
main()
  └── validate_check() : 引数チェック + init_config + .cub のパース (parse_config)
  └── setup_inits()    : init_game + finish_init
  │                      （ウィンドウ生成、テクスチャ読込、スプライト/敵リスト構築、
  │                        収集数カウント、セル属性フラグ層の構築、事前計算テーブル生成）
  └── setup_hooks()    : X11 イベントとループフックの登録 (mlx_hook / mlx_loop_hook)
  └── mlx_loop()       : 以後 main_loop() が毎フレーム呼ばれる
```

毎フレーム（`main_loop`, `srcs/core/loop.c`）では以下が走ります。

1. 経過時間から `time_mult` を算出（60 FPS 基準のスケール係数、上限 3.0 / `tuning.h` の `TARGET_FPS`・`MAX_TIME_MULT`）
2. キー入力状態に応じた移動・回転（`camera.c`）
3. アイテム取得判定 `check_quest`
4. 敵 AI 更新 `update_enemies`（**索敵 → 巡回／追跡 → テクスチャ更新**。詳細は §3）
5. `render_frame` → `update_screen` → `mlx_put_image_to_window`

## 2. ディレクトリ構成（実態）

```
cub3d/
├── Makefile
├── codes/
│   ├── includes/                      # 公開ヘッダ
│   │   ├── types.h                    # t_game ファサード、各種フラグ、入力/世界/資産/キャッシュ型
│   │   ├── tuning.h                   # コンパイル時固定の調整値・仕様定数（移動/巡回/ライト/タイル）
│   │   ├── config/
│   │   │   ├── config.h               # 設定型・マップ文字マクロ・セル属性フラグ層・解像度/色マクロ
│   │   │   └── defaults.h             # 速度/FOV/敵追跡秒/敵速度/敵HP の既定値（.cub で上書き可）
│   │   ├── core/core.h                # ライフサイクル系の公開プロトタイプ（types.h を取り込む）
│   │   ├── engine/
│   │   │   ├── input/input.h, keymap.h
│   │   │   ├── raycast/raycast.h      # t_camera, t_raysult
│   │   │   ├── render/render.h        # t_window, t_render, t_image, t_sprite
│   │   │   └── texture/texture.h
│   │   ├── enemy/
│   │   │   ├── enemy.h                # t_enemy, t_enemy_state, 公開API（リスト操作・更新）
│   │   │   └── enemy_utils.h          # 索敵/巡回/経路/移動/テクスチャの内部API・M_PI
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
│       │   ├── parse_params.c         # R / F / C とスカラー(MS/RS/FOV/ET/ES/EH)の解析
│       │   └── parse_texture.c        # NO/SO/WE/EA/ST/FT と OI1..OI5/OP1..OP5/OC1..OC5
│       ├── core/
│       │   ├── init.c                 # finish_init / init_game / init_window / find_sprites
│       │   ├── collision.c            # エンティティ当たり判定（PLAYER_RADIUS / ENEMY_RADIUS）
│       │   ├── loop.c                 # main_loop, frame_delta, calc_time_mult, 入力適用
│       │   ├── exit.c                 # 全リソース解放
│       │   ├── bmp.c                  # BMP 書き出し（フローからは未呼び出し、§8 参照）
│       │   ├── item.c                 # 収集アイテム判定 (check_quest / count_items)
│       │   └── shoot.c                # 射撃判定 (shoot_target → damage_enemy)
│       ├── enemy/                     # 敵 AI（§3 で詳述。責務ごとにファイル分割）
│       │   ├── enemy.c                # 敵リスト操作：add/delete/clear/damage
│       │   ├── enemy_ai.c             # 統括：update_enemies / 索敵→巡回・追跡の振り分け / 経路追従
│       │   ├── enemy_patrol.c         # 巡回方策：右手法則・回頭(face_angle)・巡回初期化
│       │   ├── enemy_sense.c          # 知覚：距離・視野角(FOV)・視線(LOS)の3条件判定
│       │   ├── enemy_path.c           # 経路探索：BFS（最短経路 / 最近接Pセル）
│       │   └── enemy_move.c           # 移動：dir_angle 方向へ衝突回避しつつ1フレーム前進
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
│       │       └── color.c            # シェーディング・ライト演算
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
├── maps/                              # テスト用マップ（valid/ に enemy_line.cub など）
└── textures/                          # XPM アセット
```

> **注:** 旧版にあった `ui/shortcuts.c` は廃止され、文字描画は `ui/font.c` に統合されました。`t_game` の定義は `core/core.h` から `includes/types.h` へ移設済みです。旧版の「`enemy/enemy.c` 単体で追跡 AI」という記述は古く、現在は上記のとおり責務ごとに分割されています。

## 3. 敵 AI（巡回・索敵・追跡）

敵 AI は **「毎フレーム索敵 → 状態に応じて巡回 or 追跡 → 移動 → テクスチャ更新」** という一本の流れで動きます。統括は `enemy_ai.c::update_enemies`。

### 3.1 状態と振り分け

状態は `t_enemy.state`（`enemy.h` の `t_enemy_state`）と、追跡残時間 `t_enemy.track_timer` で表現します。

| 状態 | 意味 |
|---|---|
| `ENEMY_STATE_IDLE` | 待機（巡回路へ復帰できないとき等） |
| `ENEMY_STATE_PATROL` | 巡回（`P` セル上を周回、または最近接 `P` へ復帰中） |
| `ENEMY_STATE_WALK` | 追跡（プレイヤーを検知済みで `track_timer > 0`） |
| `ENEMY_STATE_DEAD` | （撃破。実際の除去は `damage_enemy` が担当） |

振り分けは `move_enemy` がワンライナーで行います。

```c
if (track_timer > 0.0) { track_timer -= dt; state = WALK; patrol_active = 0; track_player(); }
else                   { patrol_enemy(); }
```

検知すると `track_timer` が `enemy_track_seconds`（既定 5 秒・`.cub` の `ET`）にリセットされ、見失っても残時間が尽きるまでは追跡を継続します。

### 3.2 索敵（`enemy_sense.c::enemy_sees_player`）

距離・視野角・視線の **3 条件 AND** で判定します。

1. **距離**：`hypot(dx, dy) > ENEMY_SIGHT_RANGE(=100.0)` なら不可視（デバッグ用上限）。
2. **視野角（FOV）**：`track_timer <= 0.0`（＝未追跡）のときのみ厳密チェック。プレイヤー方向 `target_angle` と敵の向き `dir_angle` の差を `(-π, π]` に正規化し、`|diff| > ENEMY_FOV_HALF(=π/8=±22.5°)` なら不可視。**追跡中は視野角ゲートを外す**（背後に回り込まれても一定時間は追える）。
3. **視線（LOS）**：`has_line_of_sight` が始点→終点を `ENEMY_LOS_STEP(=0.05 マス)` 刻みでサンプリングし、`IS_BLOCKING` セルが間にあれば遮蔽として不可視。

> **索敵タイミング（重要）：曲がり角の回頭中も索敵は実行されます。** `update_enemies` はループ先頭で **状態に関係なく無条件に** `enemy_sees_player` を呼び、その後で `move_enemy → patrol_enemy → face_angle`（回頭）に進みます。FOV 判定の基準である `dir_angle` は `face_angle` が毎フレーム旋回させるため、検知コーン（±22.5°）は回頭に追従してスイープします。
>
> なお索敵は「前フレーム終了時の `dir_angle`」を読み、その直後に `face_angle` が `dir_angle` を更新するため、検知コーンには **最大1フレーム分の位相遅れ**（標準時 1.5°／`MAX_TIME_MULT`=3 の最悪時 4.5°）があります。挙動上は無害ですが、厳密に揃えたい場合は索敵を `face_angle` 更新後に移動してください。

### 3.3 巡回（`enemy_patrol.c::patrol_enemy`）

`P` セル（`CELL_PATROL` フラグ）の上を、**右手法則**で周回します。

- **現在地が `P` 上**：`seed_patrol` で初期方向を決定（`trace_cross` が靴ひも公式でループの符号を求め、反時計回りなら逆隣へ向けて**時計回りに固定**）。到達判定 `ENEMY_PATROL_ARRIVE(=0.2)` 以内に入ったら `rh_next` が次の `P` セルを選ぶ（来た方向を基準に「左→直進→右」の優先で探索、行き止まりは来た道へ反転）。
- **現在地が `P` 外**：`bfs_to_nearest_patrol`（BFS）で最近接 `P` への次の1マスを求めて復帰。`P` が見つからなければ `ENEMY_STATE_IDLE`。
- **回頭（`face_angle`）**：目標方向との角度差を旋回速度上限 `ENEMY_TURN_DEG_PER_SEC(=90°/秒)` で詰め、**向きが揃うまでは前進せずその場で旋回**。揃った（1 を返した）フレームだけ `step_enemy` で前進します。

### 3.4 追跡（`enemy_ai.c::track_player` / 経路 `enemy_path.c`）

- `ensure_path`：プレイヤーの**セルが変わったか経路を使い切ったときだけ** `bfs_fill_path` を再計算（キャッシュは `t_enemy.path[PATH_MAX]`）。これにより毎フレームの BFS を避けます。
- `advance_path_index`：既に到達したセルを読み飛ばして次の添字へ。
- `bfs_fill_path`：4 近傍 BFS で最短経路を `path[]` に前方順で格納。経路長が `PATH_MAX(=1024)` を超える場合は始点側の先頭 `PATH_MAX` マスのみ保持し、使い切った時点で現在地から再計算（挙動は不変、再計算頻度のみ増）。`malloc` 失敗時は安全に `free` して 0 を返します。

### 3.5 移動（`enemy_move.c::step_enemy`）

`dir_angle` 方向へ `enemy_speed × speed_mult × time_mult` だけ前進。**X 軸・Y 軸を分離**して試行し、各軸ごとに `IN_MAP` / `!IS_BLOCKING` / `!is_blocked_by_entities` を満たす場合のみ反映（壁ずりが可能）。基準速度は敵専用 `enemy_speed`（`.cub` の `ES`）で、プレイヤーの `move_speed` とは独立です。当たり半径は `ENEMY_RADIUS(=0.8)` を使用（プレイヤーは `PLAYER_RADIUS(=0.5)`）。

## 4. 主要な型とデータ構造

| 型 | 役割 | 定義場所 |
|---|---|---|
| `t_game` | すべてのサブシステムを束ねるファサード | `types.h` |
| `t_config` | 解像度・色・テクスチャパス・マップ配列・**セル属性フラグ層**・速度/FOV/敵追跡秒/敵速度/敵HP | `config/config.h` |
| `t_window` | MiniLibX のポインタ、描画用バックバッファ | `render.h` |
| `t_camera` | 位置・視線・カメラ平面・直交ベクトル | `raycast.h` |
| `t_input` | 各軸の押下状態と装備中の武器・射撃状態 | `types.h` |
| `t_world` | スプライトリストと敵リスト、収集進捗 | `types.h` |
| `t_assets` | 壁/床/天井・武器・敵のテクスチャ群 | `types.h` |
| `t_render_cache` | `camera_x[MAX_WIDTH]` / `depth[MAX_WIDTH]` / `sf_dist[MAX_HEIGHT]` | `types.h` |
| `t_timing` | フレーム制御用のタイミング情報 | `types.h` |
| `t_render` | 描画関数群に渡す軽量コンテキスト（`t_game` への依存を遮断する目的） | `render.h` |
| `t_sprite` | 距離ソート用の双リンク（`next` と `sorted`） | `render.h` |
| `t_enemy` | HP・状態・**巡回状態（`patrol_*`）・追跡経路キャッシュ（`path[]` ほか）・`dir_angle`・`track_timer`** | `enemy.h` |
| `t_raysult` | 1 本のレイの計算中間結果 | `raycast.h` |

### `t_enemy` の主なフィールド（`enemy.h`）

| フィールド | 用途 |
|---|---|
| `hp` | 残ヒット数（0 で除去） |
| `state` | `t_enemy_state` |
| `patrol_active` | 巡回方向の初期化済みフラグ |
| `dir_angle` | 現在の向き（索敵 FOV と移動方向の基準） |
| `track_timer` | 追跡残時間（秒）。`> 0` で追跡継続 |
| `patrol_from` / `patrol_target` | 巡回の来た元／次の目標セル |
| `path[PATH_MAX]` / `path_idx` / `path_len` / `path_goal` / `path_valid` | 追跡経路キャッシュ |
| `sprite` | 描画用 `t_sprite*` |
| `next` | 敵リストの連結 |

### セル属性フラグ層（`config/config.h`）

訪問済みマーカー `'A'` による `map.data` の上書きから静的属性を守るため、`map.flags` という別レイヤを持ちます（起動時に一度だけ構築し以後不変）。

- `CELL_PATROL = (1 << 1)`：巡回路 `P` セル。`FLAG_XY(x, y, c)` で参照。
- ビット 0 は将来の通行可フラグ用に予約。

### オブジェクト体系（重要）

オブジェクトは **3 カテゴリ × 最大 5 種** に拡張されています（`config/config.h`）。

- マップ文字ブロック: 通行不可 `a`〜`e`（`IMP_FIRST='a'`）、通行可 `f`〜`j`（`PAS_FIRST='f'`）、収集 `k`〜`o`（`COL_FIRST='k'`）。`OBJ_PER_CATEGORY = 5`。
- 分類は `IS_IMPASSABLE` / `IS_PASSABLE` / `IS_COLLECTIBLE`、当たり判定は `IS_BLOCKING`（`'1'` または通行不可）で行います。
- マップ文字 → テクスチャスロットは `OBJ_SLOT(c)` が連番の `t_texture_id`（`TEX_IMP_1..5` / `TEX_PAS_1..5` / `TEX_COL_1..5`）を算術で引きます。
- `.cub` の設定キーは `config.c` の `g_keys[]` で定義され、`OI1..OI5` / `OP1..OP5` / `OC1..OC5` に対応します（裸の `OI`/`OP`/`OC` は廃止）。
- 有効マップ文字集合は `VALID_MAP_CHARACTERS = " 01abcdefghijklmnoEWNSMP"`（`M`=敵、`P`=巡回路）。

## 5. フレームのデータフロー

```
key_press / key_release ──► t_input
                                │
                                ▼
        main_loop ──► (入力適用) ──► t_camera 更新
                  ──► check_quest  ──► t_world.collected
                  ──► update_enemies ─► [索敵→巡回/追跡] ─► t_enemy.track_timer / dir_angle / t_sprite.pos
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

## 6. チューニング値・既定値の所在

直書きを避け、用途で分けて集約しています。

### `tuning.h`（コンパイル時固定。実行時の上書き不可）

| 区分 | 定数 | 値 | 用途 |
|---|---|---|---|
| 時間 | `TARGET_FPS` / `MAX_TIME_MULT` | 60.0 / 3.0 | FPS 非依存スケール係数の基準と上限 |
| プレイヤー | `BAREHAND_SPEED_MULT` / `PLAYER_RADIUS` | 1.3 / 0.5 | 素手時の移動倍率・当たり半径 |
| 敵 | `ENEMY_RADIUS` | 0.8 | 敵の当たり半径 |
| 敵速度 | `ENEMY_TRACK_SPEED_MULT` / `ENEMY_PATROL_SPEED_MULT` | 0.5 / 0.35 | 追跡・巡回時の速度倍率 |
| 巡回 | `ENEMY_PATROL_ARRIVE` / `ENEMY_TURN_DEG_PER_SEC` | 0.2 / 90.0 | 到達判定しきい値・回頭速度[度/秒] |
| 描画 | `BYTES_PER_PIXEL` | 4 | フォーマット上の不変条件 |
| タイル | `TILE_OBSTACLE` / `TILE_DECOR` / `TILE_ITEM` | '2' / '3' / '4' | マップタイル文字 |
| ライト | `LIGHT_CONE_DEG` / `LIGHT_RANGE` / `LIGHT_BOOST` | 20.0 / 50.0 / 1.5 | フラッシュライトの半角・距離・暗化打消量 |
| スポット | `SPOT_RADIUS` / `SPOT_GAIN` | 4.0 / 1.0 | 装飾スプライトのスポットライト半径・明度ゲイン |

### `config/defaults.h`（`.cub` で上書き可能な既定値）

| 定数 | 値 | `.cub` キー |
|---|---|---|
| `DEFAULT_MOVE_SPEED` | 0.05 | `MS` |
| `DEFAULT_ROTATE_SPEED` | 0.05 | `RS` |
| `DEFAULT_FOV` | 0.66 | `FOV` |
| `DEFAULT_ENEMY_TRACK_SECONDS` | 5.0 | `ET` |
| `DEFAULT_ENEMY_SPEED` | 0.11 | `ES` |
| `DEFAULT_ENEMY_HP` | 10.0 | `EH`（撃破に要するヒット数。整数として扱う） |

### `enemy_sense.c` 内の知覚定数（同ファイル完結）

`ENEMY_FOV_HALF(=π/8)` / `ENEMY_SIGHT_RANGE(=100.0)` / `ENEMY_LOS_STEP(=0.05)`。視野角は 8 方向スプライトの「正面」表示と同じ画角に揃えています。

## 7. ビルド

```
make           # 通常ビルド（-O3 -Wall -Wextra -Werror -I codes/includes）
make clean     # オブジェクトの削除
make fclean    # 実行ファイルも含めて削除
make re        # fclean + all
```

> **既知の制約:** 現 Makefile は `-O3` 固定で `-g` を含みません。デバッガ・サニタイザを使いたい場合は `CFLAGS` を上書きしてください（§9）。

### コーディング規約（要点）

- 変数宣言は関数頭、`for` 不使用、ヘッダガード必須。1 行 1 変数で型・名前をタブで縦整列。
- 命名: 関数・ファイルは `動詞_名詞`（`parse_map`, `draw_wall`）、構造体は `t_xxx` / `s_xxx`、列挙は `e_xxx`、マクロは `UPPER_SNAKE`。
- ヘッダには **マクロ定数・型定義・プロトタイプのみ**。実装は `.c` に閉じる。
- ポインタの `*` は型名側（`int* ptr;`）。`else` は `} else {`。関数定義はセパレータ + `//` の日本語コメントを伴う。
- 層の依存（`config` ← `core` ← `engine`/`enemy` ← `ui`）は循環させないこと。

### 付属の lint ツール（`PythonCodes/`）

```
python3 PythonCodes/lint.py            # 全検査を実行
python3 PythonCodes/lint.py --list     # 利用可能な検査の一覧
python3 PythonCodes/lint.py --fix      # 不足セパレータ/コメント雛形を挿入
```

include 順序・ポインタ表記・制御構文の空白・`else` 形・終端改行・シグネチャ・セパレータ・重複定義・マジックナンバー・未使用関数・`static` 漏洩などを検査します。

## 8. 既知の課題と TODO

実装と乖離している点、リファクタしたい点を列挙します。新しく入ってきた人はまずここを見てください。

### (a) パーサに残るデバッグ出力（解決済み）

以前 `config/config.c`・`config/parse_map.c`・`config/check_map.c` にあった `printf("DEBUG: ...")` は全て除去済みです。

### (b) `screenshot()` がエントリから呼ばれていない

`core/bmp.c` の `screenshot()` は実装済みですが、`main.c` / `input.c` のどこからも呼ばれていません（`FLAG_SAVE` フラグも定義のみで未使用）。BMP 機能を生かすなら、コマンドライン引数（例: `-save`）かキーバインドで呼び出す導線を引いてください。（※以前あった「日時付きパス `~/bmp/screenshot_YYYY_MMDD_HHMM.bmp` と固定名 `screenshot.bmp` の二重オープン／パス不一致」は解消済み: `screenshot()` が開いた `fd` を `save_bmp()` に渡し、書き込み先を日時付きパスへ統合しました。）

### (c) ヘッダの include グラフが太い

`types.h` が `config.h` / `raycast.h` / `render.h` / `enemy.h` を引き、その下流もさらに引くため、`input.c` などをビルドするだけでほぼ全ヘッダをパースします。各サブモジュールのヘッダは **公開関数の前方宣言** に絞り、構造体本体はそれを必要とする `.c` でのみ展開する整理の余地があります。

### (d) 索敵 FOV の1フレーム位相遅れ（軽微）

§3.2 のとおり、`update_enemies` は `face_angle` による `dir_angle` 更新より前に索敵します。回頭中の検知コーンが実向きに対し最大1フレーム遅れますが挙動上は無害です。厳密に揃えたい場合は索敵を `face_angle` 後へ移すか、先に回頭を1回適用してから索敵してください。

### (e) `mlx_*` 関数の戻り値 / ベンダーのバックアップ

`codes/minilibx-linux/` 配下の一部関数は `int` 宣言なのに `return` を欠き、`-Werror=return-type` を有効化すると即ビルドが落ちます。また `*.ok` 等のバックアップが残っていないか確認し、あれば `.gitignore` 除外か削除を。

## 9. 拡張のしどころ（任意）

- **巡回路（`P`）のマップ設計**: 右手法則で周回するため、閉じたループ状の `P` 配置が安定します。分岐や行き止まりも動作しますが、`seed_patrol` の周回方向固定（時計回り）を前提に設計すると意図どおりになります。
- **敵テクスチャの外部指定**: 敵は内蔵の 8 方向テクスチャを用います。`.cub` 側から指定可能にすると複数種の敵を導入できます。
- **オブジェクトのバリエーション活用**: 各カテゴリ 5 種までのテクスチャ枠を使い分けると表現が広がります。
- **HUD の充実**: HP・所持武器・残弾数の表示は未実装です。`ui.c` / `font.c` を拡張して追加できます。

## 10. 開発の始め方

```
git clone <repo>
cd cub3d
make

# 通常起動
./cub3D maps/valid/1.cub
./cub3D maps/valid/enemy_line.cub

# Address Sanitizer 付きで起動（推奨）
make fclean
make CFLAGS="-O0 -g3 -fsanitize=address -Wall -Wextra -Werror -I codes/includes"
./cub3D maps/valid/1.cub

# メモリリーク検査（MiniLibX 由来のリークは抑制対象）
valgrind --leak-check=full --suppressions=mlx.supp ./cub3D maps/valid/1.cub

# コーディングルール検査
python3 PythonCodes/lint.py
```
