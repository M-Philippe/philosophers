# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pminne <pminne@42lyon.student.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/10/23 02:26:02 by pminne            #+#    #+#              #
#    Updated: 2020/09/10 22:14:53 by pminne           ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #


CFLAGS	= -Wall -Wextra -Werror -g3 #-fsanitize=thread

NAME	=	philo_one

SRCS_D = ./srcs/

COMP	=	$(SRCS_D)main.c\
			$(SRCS_D)philosophize.c\
			$(SRCS_D)set_philosophers.c\
			$(SRCS_D)set_philosophers2.c\
			$(SRCS_D)fork.c\
			$(SRCS_D)utils.c\
			$(SRCS_D)parsing.c\
			$(SRCS_D)monitor.c\
			$(SRCS_D)print.c\

OBJ	=	$(COMP:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	clang -lpthread $(CFLAGS) -o $(NAME) $(OBJ)

%.o: %.c
	@gcc $(CFLAGS) -o $@ -c $<

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all fclean clean re
