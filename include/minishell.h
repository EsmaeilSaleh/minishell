#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef struct s_shell
{
	char	**envp;
	int	last_exit_status;
	int	running;
} t_shell;

void	init_shell(t_shell *shell, char **envp);
void	run_shell(t_shell *shell);
void	setup_signals(void);

#endif
