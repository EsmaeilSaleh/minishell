#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <fcntl.h>

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

typedef struct s_redir
{
	t_token_type type;
	char *target;
	int heredoc_fd;
	struct s_redir *next;
} t_redir;

typedef struct s_cmd
{
	char **argv;
	t_redir *redirs;
	struct s_cmd *next;
} t_cmd;

char *ft_substr(const char *s, int start, int len);
char *ft_strdup(const char *s);
int ft_strlen(const char *s);
int ft_strcmp(const char *s1, const char *s2);
int ft_atoi(const char *s);
char *ft_itoa(int n);

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

int is_pipe_token(t_token *token);
int is_redir_token(t_token *token);
int syntax_check(t_token *token);
int syntax_error(void);

t_cmd *parser(t_token *token);
t_cmd *parse_command(t_token *tokens);
int count_cmd_args(t_token *tokens);

t_redir *new_redir(t_token_type type, char *target);
void add_redir_back(t_redir **list, t_redir *new_node);

void free_cmds(t_cmd *cmds);
void free_split(char **arr);
void free_redirs(t_redir *redirs);
t_cmd *new_cmd(char **argv, t_redir *redirs);
void add_cmd_back(t_cmd **list, t_cmd *new_node);
void print_cmds(t_cmd *cmds);

char *get_env_value(char **envp, const char *name);

int execute_cmds(t_cmd *cmds, t_shell *shell);
int execute_single(t_cmd *cmd, t_shell *shell);
char *resolve_command_path(char *cmd_name, char **envp);
int ft_strncmp(const char *s1, const char *s2, size_t n);
char *ft_join_path(char *dir, char *cmd);
char **ft_split(const char *s, char c);

int is_builtin(char *cmd_name);
int exec_builtin(t_cmd *cmd, t_shell *shell);
int is_state_builtin(char *cmd_name);
int env_count(char **envp);
int env_find_index(char **envp, char *key);

int ft_echo(char **argv);
int ft_cd(char **argv, t_shell *shell);
int ft_pwd(void);
int ft_env(char **envp);
int ft_export(char **argv, t_shell *shell);
int ft_unset(char **argv, t_shell *shell);
int ft_exit(char **argv, t_shell *shell);

int apply_redirs(t_redir *redirs);
void restore_stdio(int stdin_backup, int stdout_backup);

int execute_pipeline(t_cmd *cmds, t_shell *shell);
void exec_child_process(t_cmd *cmd, t_shell *shell, int prev_fd, int pipefd[2], int has_next);

void expand_cmds(t_cmd *cmds, t_shell *shell);
char *expand_one_word(char *word, t_shell *shell);

int prepare_heredocs(t_cmd *cmds);
int prepare_heredoc(char *delimiter);
volatile sig_atomic_t g_signal_status;
#endif
