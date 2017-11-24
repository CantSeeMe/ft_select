#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jye <jye@student.42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/11/18 00:54:56 by jye               #+#    #+#              #
#    Updated: 2017/11/23 18:45:27 by jye              ###   ########.fr        #
#                                                                              #
#******************************************************************************#

CC		= gcc
CFLAGS	= -Wall -Wextra -Werror -g3
LIB		= -Lft_printf -lftprintf -Llibft -lft -lcurses

SRC_PATH = src
OBJ_PATH = obj

SRC_FILE = ft_select.c ft_select_action.c ft_select_move.c init.c qsort.c \
			signal_handle.c tools.c tools2.c yolo.c
OBJ_FILE = $(addprefix $(OBJ_PATH)/,	\
			$(addsuffix .o,				\
			$(basename $(SRC_FILE))))

NAME = ft_select

all : $(NAME)

$(NAME) : $(OBJ_FILE) ft_printf/libftprintf.a libft/libft.a
	$(CC) $(LIB) $(OBJ_FILE) -o $@

clean:
	\rm -rf obj
	make -C ft_printf fclean
	make -C libft fclean

fclean: clean
	\rm -rf $(NAME)
	\rm -rf $(NAME).dSYM

re: fclean all

libft/libft.a:
	make -C libft

ft_printf/libftprintf.a:
	make -C ft_printf

$(OBJ_PATH)/%.o : $(SRC_PATH)/%.c
	@mkdir -p $(OBJ_PATH)
	$(CC) $(CFLAGS) -c -o $@ $< -Isrc

.PHONY: fclean clean all
