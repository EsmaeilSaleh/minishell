# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dkpg-md- <dkpg-md-@student.42berlin.de>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/05/27 14:46:31 by dkpg-md-          #+#    #+#              #
#    Updated: 2026/05/28 19:25:06 by dkpg-md-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
OBJ_DIR = objects

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
       src/utils/export_utils.c \
       src/utils/run_shell_utils.c \
       src/utils/strings.c \
       src/utils/number_str.c \
       src/utils/join_split_str.c \
       src/syntax/syntax_check.c \
       src/syntax/syntax_utils.c \
       src/parser/cmd_free.c \
       src/parser/cmd_list.c \
       src/parser/parse_cmd.c \
       src/parser/parse_redir.c \
       src/parser/parser.c \
       src/exec/exec_child.c \
       src/exec/execute_pipeline.c \
       src/exec/execute.c \
       src/exec/execute_single.c \
       src/exec/execute_single_utils.c \
       src/exec/path_resolve.c \
       src/exec/redir_apply.c \
       src/builtins/builtins.c \
       src/builtins/cd.c \
       src/builtins/echo.c \
       src/builtins/env.c \
       src/builtins/exit.c \
       src/builtins/export.c \
       src/builtins/export_display.c \
       src/builtins/pwd.c \
       src/builtins/unset.c \
       src/expand/expand.c \
       src/expand/expand_utils.c \
       src/expand/expand_var.c \
       src/expand/quote_remove.c \
       src/expand/expand_args.c \
       src/heredoc/heredoc_utils.c \
       src/heredoc/heredoc.c

OBJS = $(patsubst src/%.c, $(OBJ_DIR)/%.o, $(SRCS))

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)

$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
