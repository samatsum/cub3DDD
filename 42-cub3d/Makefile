# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/10/29 12:45:02 by samatsum          #+#    #+#              #
#    Updated: 2026/05/26 22:31:39 by samatsum         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS            = main.c \
				  cub3d.c \
				  game.c \
				  bmp.c \
                  config/config.c \
				  config/map.c \
				  config/map_check.c \
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
                  engine/window.c \
				  engine/camera.c \
				  engine/shortcuts.c \
                  engine/raycast.c \
				  engine/draw.c \
				  engine/sprite.c \
                  engine/color.c \
				  engine/screen.c \
				  engine/ui.c \
                  engine/texture.c \
				  engine/tables.c \
				  engine/draw_wall.c \
                  engine/draw_sky_floor.c \
				  engine/quest.c \
                  engine/sprite_utils.c \
				  engine/crosshair.c \

OBJS            = $(SRCS:.c=.o)

CC              = gcc
RM              = rm -f
#-O は Optimization（最適化） の頭文字です。コンパイラが、人間が書いたC言語のコードを機械語のバイナリに変換する際、「いかに実行速度を速く、効率的にするか」 を突き詰める設定です。数字は、その最適化の度合い（0 〜 3）を表します。
CFLAGS          = -O3 -Wall -Wextra -Werror

NAME            = cub3D

# ============================================================================
# クロスプラットフォーム (OS判定とライブラリの動的切り替え)
# ============================================================================
UNAME_S         := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
    # Linux (X11) 環境
    MLX_DIR     = minilibx-linux
    LIBS        = -L$(MLX_DIR) -lmlx -L/usr/lib -lXext -lX11 -lm -lz
    MLX_TARGET  = $(MLX_DIR)/libmlx.a
    MLX_CLEAN   = 
else
    # macOS (Cocoa/AppKit) 環境
    MLX_DIR     = mlx
    LIBS        = -L$(MLX_DIR) -lmlx -framework OpenGL -framework AppKit -lm
    MLX_TARGET  = libmlx.dylib
    MLX_CLEAN   = $(RM) libmlx.dylib
endif

# ============================================================================
# ビルドルール
# ============================================================================

all:            $(NAME)

$(NAME):        $(MLX_TARGET) $(OBJS)
				$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBS)

$(MLX_TARGET):
				@$(MAKE) -C $(MLX_DIR)
ifeq ($(UNAME_S),Darwin)
				@mv $(MLX_DIR)/libmlx.dylib .
endif

clean:
				@$(MAKE) -C $(MLX_DIR) clean
				$(RM) $(OBJS) $(BONUS_OBJS)

fclean:         clean
				$(RM) $(NAME)
				$(MLX_CLEAN)

re:             fclean all

.PHONY:         all clean fclean re