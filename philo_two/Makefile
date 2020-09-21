# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pminne <pminne@student.42lyon.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/10/23 02:26:02 by pminne            #+#    #+#              #
#    Updated: 2020/09/21 18:50:53 by pminne           ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #


CFLAGS	= -Wall -Wextra -Werror

NAME	=	philo_two

SRCS_D = ./srcs/

COMP	=	$(SRCS_D)main.c\
			$(SRCS_D)utils.c\
			$(SRCS_D)parsing.c\
			$(SRCS_D)print.c\
			$(SRCS_D)time.c\
			$(SRCS_D)initialization.c\
			$(SRCS_D)meal.c\
			$(SRCS_D)fork.c\
			$(SRCS_D)philosophize.c\
			$(SRCS_D)meal_name.c\
			$(SRCS_D)error_semaphore.c\

OBJ	=	$(COMP:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	clang -lpthread $(CFLAGS) -o $(NAME) $(OBJ)

%.o: %.c
	clang $(CFLAGS) -I$(SRCS_D) -o $@ -c $<

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all fclean clean re
