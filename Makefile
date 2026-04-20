NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -Iinclude
LIBS = -lreadline

SRCS = src/main.c \
       src/run_shell.c \
       src/init/init_shell.c \
       src/signals/signals.c \
	   src/lexer/lexer_utils.c \
	   src/lexer/lexer.c \
	   src/lexer/token_create.c \
	   src/lexer/token_free.c \
	   src/lexer/token_list.c \
	   src/lexer/token_print.c \
	   src/utils/strings.c \
	   src/syntax/syntax_check.c \
	   src/syntax/syntax_utils.c \
	   src/parser/cmd_free.c \
	   src/parser/cmd_list.c \
	   src/parser/cmd_print.c \
	   src/parser/parse_cmd.c \
	   src/parser/parse_redir.c \
	   src/parser/parser.c \
	   src/exec/execute.c \
	   src/exec/execute_single.c \
	   src/exec/path_resolve.c \

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
