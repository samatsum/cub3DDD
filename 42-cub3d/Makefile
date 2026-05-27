# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/10/29 12:45:02 by samatsum          #+#    #+#              #
#    Updated: 2026/05/27 15:55:38 by samatsum         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME            = cub3D
CC              = gcc
RM              = rm -rf
CFLAGS          = -O3 -Wall -Wextra -Werror -I.

# オブジェクトファイルの出力先ディレクトリ
OBJ_DIR         = obj

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