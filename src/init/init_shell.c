#include "minishell.h"

void	init_shell(t_shell *shell, char **envp)
{
	shell->envp = envp;
	shell->last_exit_status = 0;
	shell->running = 1;
}
