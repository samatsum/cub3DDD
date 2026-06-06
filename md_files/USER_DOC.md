# USER_DOC - プレイヤー向けマニュアル

このドキュメントは、cub3Dの遊び方と、オリジナルの迷路（マップ）を作るためのルールをまとめたガイドです。

## 1. 遊び方 (How to Play)

makeコマンドでコンパイルし、マップファイルを指定してゲームを起動します。

```
make
./cub3D maps/マップファイル名
```

### 基本操作 (Controls)

* **W, A, S, D**: 前後左右への移動
* **Q, E または 左右矢印キー**: 視点（カメラ）の回転
* **ESC**: ゲームを終了する

### 拡張機能 (Bonus Features)

* **I**: UI（ミニマップ・進行度）の表示/非表示を切り替え
* **O**: 照準（クロスヘア）の表示/非表示を切り替え
* **L**: 距離に応じた影（シャドウ効果）のON/OFFを切り替え

---

## 2. オリジナルマップの作り方 (.cub ファイルのルール)

cub3Dでは、`.cub` 拡張子のテキストファイルを読み込んで迷路を生成します。
自分でマップを作って遊ぶ場合は、以下のルールに従ってファイルを作成してください。

### 2.1 テクスチャと色の設定

ファイルの先頭で、壁などの画像ファイルの場所と、床・天井の色（RGB）を指定します。

```
R 848 480                 : 画面の解像度（Resolution）

NO ./textures/wall/Wall_1.xpm  : 北側の壁（North）
SO ./textures/wall/Wall_2.xpm  : 南側の壁（South）
WE ./textures/wall/Wall_3.xpm  : 西側の壁（West）
EA ./textures/wall/Wall_4.xpm  : 東側の壁（East）

ST ./textures/wall/Wall_mossy.xpm   : 天井のテクスチャ（Sky Texture）
FT ./textures/wall/Wall_tile.xpm : 床のテクスチャ（Floor Texture）
F 184,113,39              : 床の色 (R,G,B)  ※床テクスチャがない場合
C 51,198,227              : 天井の色 (R,G,B)※天井テクスチャがない場合

OI ./textures/object/Object_barrel.xpm: 通行不可能な物体（Object Impossible)
OP ./textures/object/Object_greenlight.xpm: 通行可能な物体（Object Possible)
OC ./textures/object/Object_star.xpm: 通行可能な物体(コレクトアイテム)（Object Collect）

```

### 2.2 迷路の構造

設定の下に、数字とアルファベットでマップの形を描きます。

* `0`: 通行可能な床
* `1`: 通行不可能な壁
* `2`: 通行不可能な物体
* `3`: 通行可能な物体
* `4`: 通行可能な物体（コレクトアイテム）
* `N`, `S`, `E`, `W`: プレイヤーのスタート位置と向いている方角（マップに必ず1つだけ置く）

**⚠️ 注意事項:** マップの周囲は必ず壁（`1`）で完全に囲まれている必要があります。隙間があるマップはエラーとなり起動しません。