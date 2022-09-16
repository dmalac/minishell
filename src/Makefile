CC = gcc

CFLAGS = -Wall -Wextra -Werror 

RM = rm -f

NAME = minishell

LDFLAGS = -L /Users/dmonfrin/.brew/opt/readline/lib
CPPFLAGS = -I /Users/dmonfrin/.brew/opt/readline/include

LIB_PATH = ./libft/libft.a

FILES_MAIN = error_parsing.c \
parser.c \
test.c \
linked_list_moves.c \
state_machine.c \
quote_parsing.c \
raw_token_ext.c \
utility.c \
word_parsing.c \
variable.c




OBJECTS = ${FILES_MAIN:%.c=%.o}


OBJ = ${OBJECTS}
FILES = ${FILES_MAIN}


all : ${NAME}

${NAME} : ${LIB_PATH} ${OBJ}
	${CC} ${CFLAGS} ${LDFLAGS} ${CPPFLAGS} -lreadline ${OBJ} ${LIB_PATH} -o ${NAME}

${OBJ} : ${FILES}
	${CC} ${CFLAGS} -c ${FILES} 

${LIB_PATH} :
	make -C ./libft

re :	fclean all

clean :
	${RM} ${OBJECTS} ${OBJECTS_BON} ${OBJ_MAIN} ${OBJ_MAIN_BON}; make fclean -C ./libft

fclean : clean
	${RM} ${NAME} 

.PHONY: all clean fclean re 