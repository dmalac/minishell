CC = gcc

INCLDIR = incl
OBJDIR = obj
SRCDIR = src
LIBFTDIR = libft
CFLAGS = -Wall -Wextra -Werror -I ${INCLDIR} #-g -fsanitize=address

RM = rm -f

NAME = minishell

LDFLAGS = -L $(shell brew --prefix readline)/lib -lreadline
CPPFLAGS = -I $(shell brew --prefix readline)/include

LIB_PATH = ${LIBFTDIR}/libft.a

FILES_MAIN = parser.c \
parser_quote.c \
parser_word.c \
synt_error.c \
token_list_moves.c \
token_separation.c \
token_split.c \
token_utility.c \
main.c \
init.c \
symtab_add.c \
symtab_operations.c \
symtab_remove.c \
exec_get.c \
exec_child.c \
exec_parent.c \
executor.c \
exec_errors.c \
heredoc.c \
heredoc_child.c \
heredoc_utils.c \
heredoc_var_exp.c \
exec_token_processing.c \
builtin_cd.c \
builtin_echo.c \
builtin_env.c \
builtin_pwd.c \
builtin_export.c \
builtin_export_var.c \
builtin_export_lst.c \
builtin_unset.c \
builtin_exit.c \
builtin_utils.c \
var_exp.c \
var_exp_word.c \
var_exp_utility.c \
main_support.c


OBJ = ${FILES_MAIN:%.c=${OBJDIR}/%.o}
SRC = ${FILES_MAIN:%.c=${SRCDIR}/%.c}


all : ${NAME}

${NAME} : ${LIB_PATH} ${OBJ}
	${CC} ${CFLAGS} ${OBJ} ${LIB_PATH} ${LDFLAGS} ${CPPFLAGS} -o ${NAME}

${OBJDIR}/%.o : ${SRCDIR}/%.c
	${CC} ${CFLAGS} ${CPPFLAGS} -c $^ -o $@

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