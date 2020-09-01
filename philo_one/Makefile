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

CFLAGS	= -Wall -Wextra -g3 #-fsanitize=thread

NAME	=	philo_one

COMP	=	philosophers.c\
			thread.c\
			fork.c\
			timestamp.c\
			ft_itoa.c\
			parsing.c\

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
