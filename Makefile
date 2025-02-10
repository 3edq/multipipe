PROG    = pipex

SRCS    = src/pipex.c src/executer.c
OBJS    = ${SRCS:.c=.o}

HEADER  = -Isrc -Ilibft

CC      = cc
CFLAGS  = -Wall -Wextra -Werror $(HEADER)

all: $(PROG)

$(PROG): $(OBJS)
	@make -C libft
	$(CC) $(CFLAGS) $(OBJS) -Llibft -lft -o $(PROG)

clean:
	@make clean -C libft
	rm -f $(OBJS)

fclean: clean
	@make fclean -C libft
	rm -f $(PROG)

re: fclean all

.PHONY: all clean fclean re
