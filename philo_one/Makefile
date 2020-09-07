# **************************************************************************** #
#                                                           LE - /             #
#                                                               /              #
#    Makefile                                         .::    .:/ .      .::    #
#                                                  +:+:+   +:    +:  +:+:+     #
#    By: pminne <pminne@student.le-101.fr>          +:+   +:    +:    +:+      #
#                                                  #+#   #+    #+    #+#       #
#    Created: 2019/10/23 02:26:02 by pminne       #+#   ##    ##    #+#        #
#    Updated: 2019/12/20 05:48:49 by pminne      ###    #+. /#+    ###.fr      #
#                                                          /                   #
#                                                         /                    #
# **************************************************************************** #

CFLAGS	= -Wall -Wextra -Werror -g3 #-fsanitize=address

NAME	=	philo_one

SRCS_D = ./srcs/

COMP	=	$(SRCS_D)philosophers.c\
			$(SRCS_D)set_philosophers.c\
			$(SRCS_D)set_philosophers2.c\
			$(SRCS_D)fork.c\
			$(SRCS_D)ft_putnbr_fd.c\
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
