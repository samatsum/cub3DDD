# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: torinoue <torinoue@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/16 21:03:45 by torinoue          #+#    #+#              #
#    Updated: 2025/11/13 17:12:56 by torinoue         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3D

SCENE_PARSER_SRC = scene/scene_parser.c

ifneq (,$(filter bonus,$(MAKECMDGOALS)))
SCENE_PARSER_SRC = scene/scene_parse_bonus.c
endif

SRC_NAMES = util/util_error.c util/util_arr.c util/util_math.c util/util_rgb.c util/util_print.c key_events.c \
			key_events/key_events_handlers_movement.c key_events_hooks.c \
			mouse_events_hooks.c window_events_hooks.c camera_control.c scene/scene_validation.c scene/validation/color_value.c scene/validation/map/elements.c \
			init_data.c init_resources.c raycast/raycast.c raycast/raycast_distance.c scene/validation/map/closure.c scene/parse/map_loader.c load_textures.c \
			main.c $(SCENE_PARSER_SRC) scene/scene_manager.c raycast/raycast_intersection.c raycast/raycast_intersection_utils.c raycast/raycast_setup.c collision.c graphics.c graphics_texture.c scene/validation/settings/player_count.c \
			scene/validation/file_helper.c scene/validation/settings/order.c scene/validation/settings/texture_color.c
SRCS = $(addprefix src/, $(SRC_NAMES))
OBJS = $(SRCS:src/%.c=obj/%.o)
OBJS_NO_MAIN := $(filter-out obj/main.o, $(OBJS))
OBJ_DIRS = obj obj/util obj/raycast obj/scene obj/scene/parse obj/scene/validation obj/scene/validation/settings obj/scene/validation/map obj/key_events
HEADERS = $(wildcard include/*.h)
CC = cc
CFLAG = -Wall -Werror -Wextra
CFLAG_DEBUG = -g

# Additional Libraries
# Detect OS
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	MINILIBX_DIR = lib/minilibx-linux
	LIBS_SYSTEM = -L/usr/lib -lXext -lX11 -lm -lz
	SRC_NAMES += init_window_linux.c finalize_linux.c
else
	MINILIBX_DIR = lib/minilibx_opengl_20191021
	LIBS_SYSTEM = -framework OpenGL -framework AppKit -lm -lz
	SRC_NAMES += init_window_mac.c finalize_mac.c
endif
LIBFT_DIR = lib/libft
LIBPRINTF_DIR = lib/ft_printf_fd

all:			$(NAME)

bonus: all

$(OBJ_DIRS):
	mkdir -p $(OBJ_DIRS)

$(NAME): $(OBJ_DIRS) $(OBJS)
	make minilibx
	make libft
	make ft_printf
	$(CC) $(CFLAG) $(OBJS) \
	-I$(MINILIBX_DIR) \
	-I$(LIBFT_DIR) \
	-I$(LIBPRINTF_DIR) \
	-L$(MINILIBX_DIR) -lmlx \
	$(LIBS_SYSTEM) \
	-L$(LIBFT_DIR) -lft \
	-L$(LIBPRINTF_DIR) -lftprintf_fd \
	-o $(NAME)

minilibx:
	make -C $(MINILIBX_DIR)

libft:
	make -C $(LIBFT_DIR) bonus


ft_printf:
	make -C $(LIBPRINTF_DIR)

clean:
	@make -C $(LIBFT_DIR) clean
	@make -C $(LIBPRINTF_DIR) clean
	@make -C $(MINILIBX_DIR) clean 2>/dev/null || true
	$(RM) -r obj
	find src -name "*.o" -type f -delete

fclean: clean
	make -C $(LIBFT_DIR) fclean
	make -C $(LIBPRINTF_DIR) fclean
	@make -C $(MINILIBX_DIR) clean 2>/dev/null || true
	$(RM) $(NAME)

re: fclean $(NAME)


debug: CFLAG += $(CFLAG_DEBUG)
debug: $(OBJ_DIRS) $(OBJS) $(OBJS_NO_MAIN)
	rm -f $(NAME).a
	make minilibx
	make libft
	make ft_printf
	$(CC) $(CFLAG) $(OBJS) \
		-I$(MINILIBX_DIR) \
		-I$(LIBFT_DIR) \
		-I$(LIBPRINTF_DIR) \
		-L$(MINILIBX_DIR) -lmlx \
		$(LIBS_SYSTEM) \
		-L$(LIBFT_DIR) -lft \
		-L$(LIBPRINTF_DIR) -lftprintf_fd \
		-o $(NAME)
	ar rcs $(NAME).a $(OBJS_NO_MAIN)

re_debug: fclean debug

# Target for AddressSanitizer
sanitize: CFLAG += -fsanitize=address -g -O1
sanitize: $(OBJ_DIRS) $(OBJS)
	make minilibx
	make libft
	make ft_printf
	$(CC) $(CFLAG) $(OBJS) \
	-I$(MINILIBX_DIR) \
	-I$(LIBFT_DIR) \
	-I$(LIBPRINTF_DIR) \
	-L$(MINILIBX_DIR) -lmlx \
	$(LIBS_SYSTEM) \
	-L$(LIBFT_DIR) -lft \
	-L$(LIBPRINTF_DIR) -lftprintf_fd \
	-fsanitize=address \
	-o $(NAME)

obj/%.o: src/%.c $(HEADERS)
	$(CC) $(CFLAG) -c $< -o $@ \
	-I$(MINILIBX_DIR) \
	-I$(LIBFT_DIR) \
	-I$(LIBPRINTF_DIR) \
	-Iinclude

# Documentation generation
# Try Doxygen first, fallback to Python script if not available
docs:
	@if command -v doxygen >/dev/null 2>&1; then \
		echo "Generating documentation with Doxygen..."; \
		doxygen Doxyfile; \
		echo "Documentation generated in docs/doxygen/html/"; \
		echo "Open docs/doxygen/html/index.html in your browser to view the documentation."; \
	elif command -v python3 >/dev/null 2>&1; then \
		echo "Doxygen not found. Using Python script instead..."; \
		python3 generate_docs.py; \
		echo "Documentation generated in docs/html/"; \
		echo "Open docs/html/index.html in your browser to view the documentation."; \
	else \
		echo "Error: Neither Doxygen nor Python3 is available."; \
		echo "Install Doxygen with: sudo apt-get install doxygen (Ubuntu/Debian)"; \
		echo "                  or: brew install doxygen (macOS)"; \
		exit 1; \
	fi

docs-clean:
	@rm -rf docs/doxygen docs/html
	@echo "Documentation cleaned."

.PHONY: all clean fclean re minilibx libft ft_printf debug re_debug sanitize docs docs-clean bonus
