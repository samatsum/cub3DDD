NAME		= cub3D
CC 			= cc
CFLAGS		= -Wall -Wextra -Werror -g
MLX_DIR		= ./minilibx-linux
MLX_LIB		= libmlx.a
INCLUDES	= -I$(MLX_DIR) -Iinclude
LIBS		= -L$(MLX_DIR) -lmlx -lXext -lX11 -lm

SRCS_UTILS	= src/utils/ft_atoi.c \
			src/utils/ft_isdigit.c \
			src/utils/ft_isspace.c \
			src/utils/ft_memcpy.c \
			src/utils/ft_memmove.c \
			src/utils/ft_memset.c \
			src/utils/ft_realloc.c \
			src/utils/ft_split.c \
			src/utils/ft_strchr.c \
			src/utils/ft_strcpy.c \
			src/utils/ft_strdup.c \
			src/utils/ft_strjoin.c \
			src/utils/ft_strlen.c \
			src/utils/ft_strncmp.c \
			src/utils/ft_strlcpy.c \
			src/utils/ft_substr.c \
			src/utils/get_next_line.c

SRCS_INIT	= src/init/init.c \
			src/init/map_init.c \
			src/init/map_free.c \
			src/init/content_init.c

SRCS_RAY	= src/raycasting/get_move_input.c \
			src/raycasting/key_event.c \
			src/raycasting/raycasting.c \
			src/raycasting/render_fram.c \
			src/raycasting/render_util.c \
			src/raycasting/render.c

SRCS_VALID	= src/valid/dfs.c \
			src/valid/map_deep_copy.c \
			src/valid/valid.c

SRCS_PRECHECK	= src/precheck/precheck_color.c \
			src/precheck/precheck_init_finish.c \
			src/precheck/precheck_texture.c \
			src/precheck/precheck_utils.c \
			src/precheck/precheck.c

SRCS		= $(SRCS_UTILS) $(SRCS_INIT) $(SRCS_RAY) $(SRCS_VALID) $(SRCS_PRECHECK) src/main.c
OBJS		= $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS) $(MLX_DIR)/$(MLX_LIB)
	$(CC) $(OBJS) $(LIBS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(MLX_DIR)/$(MLX_LIB):
	$(MAKE) -C $(MLX_DIR)

clean:
	rm -rf $(OBJS)
	$(MAKE) -C $(MLX_DIR) clean


fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(MLX_DIR) clean

re: fclean all

.PHONY: all clean fclean re
