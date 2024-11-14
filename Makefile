NAME		= pipex
CC			= cc
CFLAGS		= -Wextra -Wall -Werror -g #-O0 -Ofast #-fsanitize=address -Wunreachable-code
LIBFT		= ./libft

HEADERS	= -Iincludes

SRCS	= get_cmd_path.c \
		  child_processes.c \
		  child_utils.c \
		  main.c

OBJS	= $(SRCS:.c=.o)

all: $(NAME)

$(LIBFT)/libft.a:
	make -C $(LIBFT)
	make bonus -C $(LIBFT)

%.o: %.c
	$(CC) $(CFLAGS) $(HEADERS) -o $@ -c $<

$(NAME): $(OBJS) $(LIBFT)/libft.a
	$(CC) $(OBJS) $(LIBFT)/libft.a -o $(NAME) 

clean:
	rm -rf $(OBJS)
	make -C $(LIBFT) fclean

fclean: clean
	rm -rf $(NAME)

re: clean all

.PHONY: re fclean clean all
