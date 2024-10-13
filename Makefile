NAME		= pipex
CC			= cc
CFLAGS		= -Wextra -Wall -Werror #-O0 -Ofast -g #-fsanitize=address -Wunreachable-code
FT_PRINTF	= ./ft_printf
LIBFT		= ./libft

HEADERS	= -I $(FT_PRINTF)/include

SRCS	= main.c utils.c

OBJS	= $(SRCS:.c=.o)

all: $(NAME)

$(LIBFT)/libft.a:
	make -C $(LIBFT)

$(FT_PRINTF)/libftprintf.a:
	make -C $(FT_PRINTF)

%.o: %.c
	$(CC) $(CFLAGS) $(HEADERS) -o $@ -c $<

$(NAME): $(OBJS) $(LIBFT)/libft.a $(FT_PRINTF)/libftprintf.a
	$(CC) $(OBJS) $(LIBFT)/libft.a $(FT_PRINTF)/libftprintf.a -o $(NAME) 

clean:
	rm -rf $(OBJS)
	make -C $(LIBFT) fclean
	make -C $(FT_PRINTF) fclean

fclean: clean
	rm -rf $(NAME)

re: clean all

.PHONY: re fclean clean all
