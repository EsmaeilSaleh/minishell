#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef struct s_shell
{
	char **envp;
	int last_exit_status;
	int running;
} t_shell;

typedef enum e_token_type
{
	TOK_WORD,
	TOK_PIPE,
	TOK_REDIR_IN,
	TOK_REDIR_OUT,
	TOK_APPEND,
	TOK_HEREDOC
} t_token_type;

typedef struct s_token
{
	char *value;
	t_token_type type;
	struct s_token *next;
} t_token;

void init_shell(t_shell *shell, char **envp);
void run_shell(t_shell *shell);
void setup_signals(void);

t_token *lexer(char *line);
void print_tokens(t_token *tokens);
void free_tokens(t_token *tokens);

t_token *new_token(char *value, t_token_type type);
void add_token_back(t_token **list, t_token *new_node);
int is_space(char c);
int is_operator_char(char c);
char *extract_word(char *line, int *i);

char *ft_substr(const char *s, unsigned int start, int len);
char *ft_strdup(char *s);

#endif
