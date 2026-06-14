NAME            = cub3D
CC              = gcc
RM              = rm -rf

# ==============================================================================
# ディレクトリ設定
# ==============================================================================
SRC_DIR         = codes/srcs
INC_DIR         = codes/includes
OBJ_DIR         = codes/obj

# ------------------------------------------------------------------------------
# コンパイルフラグ (CFLAGS)
#   -O3            : 最適化レベル3（速度優先。デバッグ時は debug ターゲットで -O0 に上書き）
#   -Wall          : 一般的な警告を有効化
#   -Wextra        : -Wall に含まれない追加の警告も有効化
#   -Werror        : すべての警告をエラー扱いにし、警告ゼロを強制する
#   -I $(INC_DIR)  : 自作ヘッダの探索パス（#include "..." の基準に codes/includes を追加）
# ------------------------------------------------------------------------------
CFLAGS          = -O3 -Wall -Wextra -Werror -I $(INC_DIR)

# ==============================================================================
# ソースファイル定義
# ==============================================================================
SRCS_LIST       = main.c \
                  core/item.c \
                  core/bmp.c \
                  core/init.c \
                  core/exit.c \
                  core/loop.c \
                  core/shoot.c \
                  core/collision.c \
                  enemy/enemy.c \
                  enemy/enemy_ai.c \
                  enemy/enemy_assets.c \
                  enemy/enemy_sense.c \
                  enemy/enemy_path.c \
                  enemy/enemy_move.c \
                  enemy/enemy_patrol.c \
                  config/config.c \
                  config/parse_map.c \
                  config/check_map.c \
                  config/parse_params.c \
                  config/parse_texture.c \
                  utils/ft_strlen.c \
                  utils/ft_substr.c \
                  utils/ft_in_set.c \
                  utils/str.c \
                  utils/ft_strdup.c \
                  utils/ft_split.c \
                  utils/ft_atoi.c \
                  utils/pos.c \
                  utils/ft_itoa.c \
                  utils/ft_memmove.c \
                  utils/ft_strcmp.c \
                  utils/ft_write.c \
                  utils/ft_endwith.c \
                  gnl/get_next_line.c \
                  gnl/get_next_line_utils.c \
                  engine/raycast/camera.c \
                  engine/raycast/raycast.c \
                  engine/raycast/tables.c \
                  engine/render/draw.c \
                  engine/render/draw_wall.c \
                  engine/render/draw_weapon.c \
                  engine/render/draw_sky_floor.c \
                  engine/render/screen.c \
                  engine/render/sprite.c \
                  engine/render/sprite_utils.c \
                  engine/texture/color.c \
                  engine/texture/texture.c \
                  engine/input/input.c \
                  ui/ui.c \
                  ui/font.c \
                  ui/crosshair.c

SRCS            = $(addprefix $(SRC_DIR)/, $(SRCS_LIST))
OBJS            = $(addprefix $(OBJ_DIR)/, $(SRCS_LIST:.c=.o))

# ==============================================================================
# Linux(X11) ライブラリ設定
# ==============================================================================
MLX_DIR         = codes/minilibx-linux

# ------------------------------------------------------------------------------
# リンクするライブラリ (LIBS) ※リンク時に実行ファイルへ結合する外部ライブラリ群
#   -L$(MLX_DIR) : ライブラリ探索パスに MiniLibX のディレクトリを追加（libmlx.a の場所）
#   -lmlx        : MiniLibX 本体（X11 上に薄い描画 API を提供するベンダーライブラリ）
#   -L/usr/lib   : ライブラリ探索パスに /usr/lib を追加（X11 等の標準的な配置先）
#   -lXext       : X11 拡張ライブラリ（共有メモリ XShm など。MiniLibX が依存）
#   -lX11        : X11 コアライブラリ（ウィンドウ生成・イベント・描画の土台）
#   -lm          : 数学ライブラリ（floor / sin / cos / sqrt など。レイキャスト計算で使用）
#   -lz          : zlib（圧縮。MiniLibX の XPM/画像処理まわりが依存）
#
#   注意: リンク順序は意味を持つ。-lmlx は自身が必要とする -lXext -lX11 より「前」に
#         置く（依存される側を後ろに並べる）。-L は対応する -l より前に置くこと。
# ------------------------------------------------------------------------------
LIBS            = -L$(MLX_DIR) -lmlx -L/usr/lib -lXext -lX11 -lm -lz
MLX_TARGET      = $(MLX_DIR)/libmlx.a

# ==============================================================================
# ビルドルール
# ==============================================================================
all:            $(NAME)

$(NAME):        $(MLX_TARGET) $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# minilibx は自前の穏やかなフラグ(-O3)でビルドする。
# CFLAGS をサブ make に渡さないこと（渡すと下流の test/configure まで
# インクルードパスごと上書きされて壊れる）。
$(MLX_TARGET):
	@$(MAKE) -C $(MLX_DIR)

# ==============================================================================
# デバッグ / サニタイザビルド
#   target 固有変数として CFLAGS を追加するため、minilibx サブ make には
#   伝播しない（コマンドラインでの CFLAGS 上書きと違い、本体だけに効く）。
#   -static-libasan: ASan ランタイムを静的リンクし、共有 libasan の
#   「初期化順が先頭でない」エラー(LD_PRELOAD 要求)を回避する。
#   使い方: make debug && ./cub3D maps/valid/1.cub
# ==============================================================================
debug:          CFLAGS += -O0 -g3 -fsanitize=address -static-libasan
debug:          re

# ==============================================================================
# 品質担保（静的解析・Linter群）
# ==============================================================================
check:
	@echo "=================================================="
	@echo " Running Static Analysis & Quality Checks... "
	@echo "=================================================="
	@python3 codes/PythonCodes/lint.py
	@echo "=================================================="
	@echo " All checks passed! Code is perfectly structured. "
	@echo "=================================================="

clean:
	-@$(MAKE) -C $(MLX_DIR) clean
	$(RM) $(OBJ_DIR)

fclean:         clean
	$(RM) $(NAME)

re:             fclean all

.PHONY:         all clean fclean re check debug
