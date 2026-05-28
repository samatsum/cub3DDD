NAME            = cub3D
CC              = gcc
RM              = rm -rf

# ==============================================================================
# ディレクトリ設定
# ==============================================================================
SRC_DIR         = codes/srcs
INC_DIR         = codes/includes
OBJ_DIR         = codes/obj

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
                  engine/render/draw_sky_floor.c \
                  engine/render/screen.c \
                  engine/render/sprite.c \
                  engine/render/sprite_utils.c \
                  engine/texture/color.c \
                  engine/texture/texture.c \
                  engine/input/input.c \
                  ui/shortcuts.c \
                  ui/ui.c \
                  ui/crosshair.c

SRCS            = $(addprefix $(SRC_DIR)/, $(SRCS_LIST))
OBJS            = $(addprefix $(OBJ_DIR)/, $(SRCS_LIST:.c=.o))

# ==============================================================================
# Linux(X11) ライブラリ設定
# ==============================================================================
MLX_DIR         = codes/minilibx-linux
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

$(MLX_TARGET):
				@$(MAKE) -C $(MLX_DIR)

clean:
				@$(MAKE) -C $(MLX_DIR) clean
				$(RM) $(OBJ_DIR)

fclean:         clean
				$(RM) $(NAME)

re:             fclean all

.PHONY:         all clean fclean re