# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: toruinoue <toruinoue@student.42.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/23 21:37:50 by torinoue          #+#    #+#              #
#    Updated: 2025/10/25 23:50:10 by toruinoue        ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libftprintf_fd.a
SRCS = \
	   ft_strlen_fd.c \
	   ft_printf_fd.c \
	   put_cspdiux_fd.c \

OBJS_DIR = objs
OBJS = $(patsubst %.c, $(OBJS_DIR)/%.o, $(SRCS))
CC = cc
CFLAGS = -Wall -Wextra -Werror
MKDIR = mkdir -p
INCLUDE = ft_printf_fd.h
DEBUGFLAGS = -g -fsanitize=address
ifeq ($(MAKECMDGOALS), debug)
	CFLAGS += $(DEBUGFLAGS)
endif
RM = rm -rf

all: $(NAME)

$(NAME): $(OBJS)
	ar -rcs $(NAME) $(OBJS)

bonus: $(NAME)

$(OBJS_DIR)/%.o: %.c $(INCLUDE) | $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR):
	$(MKDIR) $(OBJS_DIR)

clean:
	$(RM) $(OBJS_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

debug: all

.PHONY: all clean fclean re debug bonus