# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/10/29 12:45:02 by samatsum          #+#    #+#              #
#    Updated: 2026/05/27 19:56:59 by samatsum         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME            = cub3D
CC              = gcc
RM              = rm -rf
CFLAGS          = -O3 -Wall -Wextra -Werror -I ./include

# オブジェクトファイルの出力先ディレクトリ
OBJ_DIR         = obj

SRCS            = src/main.c \
                  src/core/cub3d.c \
                  src/core/game.c \
                  src/core/bmp.c \
                  src/core/init.c \
                  src/core/exit.c \
                  src/core/loop.c \
                  src/config/config.c \
                  src/config/map.c \
                  src/config/map_check.c \
                  src/config/parse_params.c \
                  src/config/parse_texture.c \
                  src/utils/ft_strlen.c \
                  src/utils/ft_substr.c \
                  src/utils/ft_in_set.c \
                  src/utils/str.c \
                  src/utils/ft_strdup.c \
                  src/utils/ft_split.c \
                  src/utils/ft_atoi.c \
                  src/utils/pos.c \
                  src/utils/ft_itoa.c \
                  src/utils/ft_memmove.c \
                  src/utils/ft_strcmp.c \
                  src/utils/ft_write.c \
                  src/utils/ft_endwith.c \
                  src/gnl/get_next_line.c \
                  src/gnl/get_next_line_utils.c \
                  src/engine/math/camera.c \
                  src/engine/math/raycast.c \
                  src/engine/math/tables.c \
                  src/engine/render/draw.c \
                  src/engine/render/draw_wall.c \
                  src/engine/render/draw_sky_floor.c \
                  src/engine/render/screen.c \
                  src/engine/render/sprite.c \
                  src/engine/render/sprite_utils.c \
                  src/engine/texture/color.c \
                  src/engine/texture/texture.c \
                  src/engine/input/hooks.c \
                  src/ui/shortcuts.c \
                  src/ui/ui.c \
                  src/ui/crosshair.c

# オブジェクトファイルのリストを作成
OBJS            = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

# OS判定
UNAME_S         := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
    MLX_DIR     = minilibx-linux
    LIBS        = -L$(MLX_DIR) -lmlx -L/usr/lib -lXext -lX11 -lm -lz
    MLX_TARGET  = $(MLX_DIR)/libmlx.a
    MLX_CLEAN   = 
else
    MLX_DIR     = mlx
    LIBS        = -L$(MLX_DIR) -lmlx -framework OpenGL -framework AppKit -lm
    MLX_TARGET  = libmlx.dylib
    MLX_CLEAN   = $(RM) libmlx.dylib
endif

# ビルドルール
all:            $(NAME)

$(NAME):        $(MLX_TARGET) $(OBJS)
				$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBS)

# オブジェクトファイルの生成ルール
$(OBJ_DIR)/%.o: %.c
				@mkdir -p $(@D)
				$(CC) $(CFLAGS) -c $< -o $@

$(MLX_TARGET):
				@$(MAKE) -C $(MLX_DIR)
ifeq ($(UNAME_S),Darwin)
				@mv $(MLX_DIR)/libmlx.dylib .
endif

clean:
				@$(MAKE) -C $(MLX_DIR) clean
				$(RM) $(OBJ_DIR) $(BONUS_OBJS)

fclean:         clean
				$(RM) $(NAME)
				$(MLX_CLEAN)

re:             fclean all

.PHONY:         all clean fclean re