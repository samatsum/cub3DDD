_This project has been created as part of the 42 curriculum by hyamamot, oo_

[English](README.md) | [日本語](README.ja.md)

## 目次

- [概要](#概要)
- [使い方](#使い方)
- [参考資料](#参考資料)

---

## 概要

### プロジェクトについて
**cub3d**は、42カリキュラムのプロジェクトの一つです。   
1992年にId Softwareが開発したFPSゲーム *Walfenstein 3D*にインスパイアされ、C言語でレイキャスティングを用いた擬似3D ビューアを実装します。


### 機能概要

| 機能 | 内容 |
|---|---|
| 3D レンダリング | レイキャスティングによる疑似 3D 一人称視点 |
| テクスチャ | 北・南・東・西の壁面にそれぞれ XPM テクスチャを貼り付け |
| 天井・床の色 | `.cub` ファイルで RGB カラーを指定 |
| プレイヤー移動 | `W` `A` `S` `D` で前後左右移動 |
| 視点回転 | 左右矢印キーで回転 |
| マップ検証 | DFS による閉鎖マップの整合性チェック |
| 終了 | `ESC` キーまたはウィンドウの × ボタン |

### ディレクトリ構成

```
cub3d/
├── include/        ヘッダファイル
├── src/
│   ├── init/       .cubファイルのパース・マップ初期化
│   ├── raycasting/ メインループ・レイキャスト・描画
│   ├── utils/      自作ライブラリ関数 (ft_*) と get_next_line
│   └── valid/      マップ検証 (DFS)
├── texture/        XPM テクスチャ画像
└── in.cub          サンプルマップファイル
```

### `.cub` ファイルの書式

```
NO ./texture/north.xpm   # 北壁テクスチャ
SO ./texture/south.xpm   # 南壁テクスチャ
WE ./texture/west.xpm    # 西壁テクスチャ
EA ./texture/east.xpm    # 東壁テクスチャ

F 220,100,0              # 床の色 (R,G,B)
C 225,30,0               # 天井の色 (R,G,B)

11111                    # マップ (1=壁, 0=通路, N/S/E/W=初期位置と向き)
10N01
11111
```

---

## 使い方

### 依存関係

- `gcc`
- `make`
- X11 開発ライブラリ: `libx11-dev`, `libxext-dev`
- MiniLibX (リポジトリ内 `/workspace/hyamamot/minilibx-linux` を使用)

```bash
# Ubuntu / Debian の場合
sudo apt-get install gcc make libx11-dev libxext-dev
```

### コンパイル

```bash
cd cub3d
make
```

### 実行

```bash
./cub3d <マップファイル.cub>

# 例
./cub3d in.cub
```

### クリーン

```bash
make clean    # オブジェクトファイルを削除
make fclean   # オブジェクトファイルと実行ファイルを削除
make re       # 再ビルド
```

### 操作方法

| キー | 動作 |
|---|---|
| `W` | 前進 |
| `S` | 後退 |
| `A` | 左ストレーフ |
| `D` | 右ストレーフ |
| `←` | 左回転 |
| `→` | 右回転 |
| `ESC` | 終了 |

---

## 参考資料

| # | タイトル | 用途 |
|---|---|---|
| [1] | [Cub3D - C言語で3DFPSゲーム作ってみた](https://zenn.dev/rt3mis10/articles/3482cc49e288ea) | 同課題の実装例・全体設計の参考 |
| [2] | [MiniLibX document](https://harm-smits.github.io/42docs/libs/minilibx) | MiniLibX API リファレンスとサンプルコード |
| [3] | [Adobe: レイキャスティングとは？](https://www.adobe.com/jp/products/substance3d/discover/what-is-ray-casting.html) | レイキャスティングの概念説明 |
| [4] | [Lode's Raycasting Tutorial](https://lodev.org/cgtutor/raycasting.html) | レイキャスティングアルゴリズムの主要参考文献 |
| [5] | [釧路高専: レイキャスティング](https://www.kushiro-ct.ac.jp/yanagawa/cg-2015/1403/index.html) | 日本語によるレイキャスティング解説 |
| [6] | [GitHub: raycasting Prototype with DDA](https://github.com/GPMLAD/raycasting) | DDA を使ったレイキャスティングの実装例 |
| [7] | [釧路高専: 線分の描画 DDA](https://www.kushiro-ct.ac.jp/yanagawa/cg/03/index.html) | DDA アルゴリズムの詳細解説 |
| [8] | [Qiita: C言語でFPS非依存アニメーション](https://qiita.com/Tanoren/items/10a4f610b9d3c5f744d0) | フレームタイムを用いたフレームレート非依存の移動速度制御 |
| [9] | [DeepWiki: cub3d Flood Fill Algorithm](https://deepwiki.com/eltitoyisus/cub3D/3.2-flood-fill-algorithm) | マップ検証 (DFS/Flood Fill) の実装参考 |
| [10] | [C言語で3Dゲームを作った](https://jun-networks.hatenablog.com/entry/2021/03/04/130629) | C言語による3Dゲーム実装の解説記事 |

### AI の活用について

本プロジェクトでは **GitHub Copilot (Claude Sonnet 4.6)** を以下の用途で活用しました。

| 用途 | 詳細 |
|---|---|
| コードのリファクタリング | ノルミネット違反（キャメルケース変数名・行長超過・スコープ外変数宣言）の一括修正 |
| 命名規則の統一 | `posX/Y`, `dirX/Y`, `planeX/Y` などのキャメルケースをスネークケースに変換し、全参照箇所へ反映 |
| バグ修正 | `continue` とループ末尾 `gnl` の組み合わせによる無限ループの発見と修正 |

アルゴリズムの設計・実装の本質的な部分（レイキャスティング、DFS、マップパーサ）は自力で実装し、AI は主に規約準拠・デバッグ・ドキュメント整備に活用しました。
