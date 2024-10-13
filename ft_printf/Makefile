#Name of the library
NAME = libftprintf.a

#Executable name
EXECUTABLE = test

#Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror
#-Wsign-conversion

#Directory locations
INC_DIR = ./include
SRC_DIR = ./src

#Finding all .c files in src directory
SRC_SOURCES = $(SRC_DIR)/ft_printf.c \
			  $(SRC_DIR)/ft_putnbr.c \
			  $(SRC_DIR)/ft_putstr.c \
			  $(SRC_DIR)/ft_putchar.c \
			  $(SRC_DIR)/ft_putnbr_u.c \
			  $(SRC_DIR)/ft_strlen.c

#Converting .c filenames to .o
SRC_OBJECTS = $(SRC_SOURCES:.c=.o)

#Default target
all: $(NAME)

#Executable rule
exe: $(EXECUTABLE)

#Creating the static library from both src and libft object files
$(NAME): $(SRC_OBJECTS) 
	ar rcs $@ $^

#Pattern rule for object files in src and libft directories
%.o: %.c
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

#Linking all object files to create the executable
$(EXECUTABLE): $(SRC_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

#Cleaning up object files in both src and libft directories
clean:
	rm -f $(SRC_DIR)/*.o

#Cleaning everything that has been created, including the library
fclean: clean
	rm -f $(NAME)
	rm -f $(EXECUTABLE)

#Re-make everything
re: fclean all

#Mark rules as phony
.PHONY: all clean fclean re exe
