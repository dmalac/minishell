CC = gcc

INCLDIR = incl
OBJDIR = obj
SRCDIR = src
LIBFTDIR = libft
CFLAGS = -Wall -Wextra -Werror -I ${INCLDIR} -g -fsanitize=address

RM = rm -f

NAME = minishell

# LDFLAGS = -L /Users/dmonfrin/.brew/opt/readline/lib
# CPPFLAGS = -I /Users/dmonfrin/.brew/opt/readline/include
LDFLAGS = -L /Users/dmalacov/.brew/opt/readline/lib
CPPFLAGS = -I /Users/dmalacov/.brew/opt/readline/include

LIB_PATH = ${LIBFTDIR}/libft.a

FILES_MAIN = error_parsing.c \
parser.c \
test.c \
linked_list_moves.c \
state_machine.c \
quote_parsing.c \
raw_token_ext.c \
utility.c \
word_parsing.c \
variable.c \
init.c \
symtab.c \
exec_get.c \
exec_child.c \
exec_parent.c \
executor.c \
exec_errors.c \
heredoc.c \
token_processing.c \
builtin_cd.c \
builtin_echo.c \
builtin_env.c \
builtin_pwd.c \
builtin_export.c \
builtin_unset.c \
builtin_exit.c \
builtin_utils.c


OBJ = ${FILES_MAIN:%.c=${OBJDIR}/%.o}
SRC = ${FILES_MAIN:%.c=${SRCDIR}/%.c}


all : ${NAME}

${NAME} : ${LIB_PATH} ${OBJ}
	${CC} ${CFLAGS} ${LDFLAGS} ${CPPFLAGS} -lreadline ${OBJ} ${LIB_PATH} -o ${NAME}

${OBJDIR}/%.o : ${SRCDIR}/%.c
	${CC} ${CFLAGS} -c $^ -o $@

${OBJ} : | ${OBJDIR}

${OBJDIR} :
	mkdir $@

${LIB_PATH} :
	make -C ${LIBFTDIR}

re :	fclean all

clean :
	${RM} -R ${OBJDIR}
	make fclean -C ${LIBFTDIR}

fclean : clean
	${RM} ${NAME} 

.PHONY: all clean fclean re 