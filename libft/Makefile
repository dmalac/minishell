NAME = libft.a

SRC = ft_atol.c \
ft_bzero.c \
ft_isalnum.c \
ft_isalpha.c \
ft_itoa.c \
ft_putendl_fd.c \
ft_putstr_fd.c \
ft_split.c \
ft_strdup.c \
ft_strjoin.c \
ft_strjoin_env.c \
ft_strlcat.c \
ft_strlcpy.c \
ft_strlen.c \
ft_strncmp.c \
ft_strnstr.c \
ft_substr.c

INCLDIR = ../incl
OBJDIR = obj
OBJ = $(SRC:%.c=$(OBJDIR)/%.o)

CC = gcc
CC_FLAGS = -Wall -Wextra -Werror -I $(INCLDIR) -g -fsanitize=address

all: $(NAME)

$(NAME): $(OBJ)
	ar -rsu $@ $?

$(OBJ): | $(OBJDIR)

$(OBJDIR)/%.o: %.c
	$(CC) $(CC_FLAGS) -c $? -o $@

$(OBJDIR):
	mkdir $@

clean:
	rm -Rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re