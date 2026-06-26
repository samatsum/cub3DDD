NAME            = cub3D
CC              = gcc
RM              = rm -rf

# ==============================================================================
# ディレクトリ設定（srcs を common / fps / rsp の3系統に分割）
# ==============================================================================
INC_DIR         = codes/includes
OBJ_DIR         = codes/obj
COMMON_DIR      = codes/srcs/common
FPS_DIR         = codes/srcs/fps
RSP_DIR         = codes/srcs/rsp

# ------------------------------------------------------------------------------
# コンパイルフラグ（ヘッダは codes/includes に集約。rsp 系も codes/includes/rsp 配下）
# ------------------------------------------------------------------------------
CFLAGS          = -O3 -Wall -Wextra -Werror -pthread -I $(INC_DIR)

# ==============================================================================
# ソース定義
# ==============================================================================
COMMON_SRCS     = main.c \
                  config/config.c config/parse_map.c config/check_map.c \
                  config/parse_params.c config/parse_texture.c \
                  utils/ft_strlen.c utils/ft_substr.c utils/ft_in_set.c \
                  utils/str.c utils/ft_strdup.c utils/ft_split.c utils/ft_atoi.c \
                  utils/pos.c utils/ft_strcmp.c utils/ft_rand.c \
                  utils/ft_write.c utils/ft_endswith.c \
                  gnl/get_next_line.c gnl/get_next_line_utils.c \
                  engine/raycast/camera.c engine/raycast/raycast.c \
                  engine/raycast/spawn.c engine/raycast/spawn_marker.c \
                  engine/render/draw.c engine/render/draw_wall.c \
                  engine/render/draw_sky_floor.c engine/render/screen.c \
                  engine/render/sprite.c engine/render/sprite_utils.c \
                  engine/render/cast_columns.c engine/render/light.c \
                  engine/render/tables.c engine/render/draw_weapon.c \
                  engine/texture/color.c engine/texture/texture.c \
                  engine/input/input.c \
                  core/collision.c core/bmp.c \
                  core/loop.c core/init.c core/exit.c core/respawn.c \
                  enemy/enemy.c \
                  ui/font.c ui/ui.c ui/crosshair.c

FPS_SRCS        = core/shoot.c core/item.c core/respawn.c core/mode_fps.c core/assets.c \
                  enemy/enemy_ai.c enemy/enemy_assets.c \
                  enemy/enemy_sense.c enemy/enemy_path.c enemy/enemy_move.c \
                  enemy/enemy_patrol.c \
                  render/weapon_fps.c

RSP_SRCS        = rsp_rule.c rsp_assets.c rsp_combat.c rsp_setup.c rsp_ai.c \
                  rsp_weapon.c rsp_mode.c

OBJS            = $(addprefix $(OBJ_DIR)/common/, $(COMMON_SRCS:.c=.o)) \
                  $(addprefix $(OBJ_DIR)/fps/, $(FPS_SRCS:.c=.o)) \
                  $(addprefix $(OBJ_DIR)/rsp/, $(RSP_SRCS:.c=.o))

# ==============================================================================
# Linux(X11) ライブラリ設定
# ==============================================================================
MLX_DIR         = codes/minilibx-linux
LIBS            = -L$(MLX_DIR) -lmlx -L/usr/lib -lXext -lX11 -lm -lz
MLX_TARGET      = $(MLX_DIR)/libmlx.a

# ==============================================================================
# ビルドルール（root ごとに1つずつ）
# ==============================================================================
all:            $(NAME)

$(NAME):        $(MLX_TARGET) $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBS)

$(OBJ_DIR)/common/%.o: $(COMMON_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/fps/%.o: $(FPS_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/rsp/%.o: $(RSP_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(MLX_TARGET):
	@$(MAKE) -C $(MLX_DIR)

# ==============================================================================
# デバッグ / 品質担保 / クリーン
# ==============================================================================
debug:          CFLAGS += -O0 -g3 -fsanitize=address -static-libasan
debug:          re

check:
	@python3 codes/PythonCodes/lint.py

clean:
	-@$(MAKE) -C $(MLX_DIR) clean
	$(RM) $(OBJ_DIR)

fclean:         clean
	$(RM) $(NAME)

re:             fclean all

.PHONY:         all clean fclean re check debug
