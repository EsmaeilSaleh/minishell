#include "minishell.h"

int main(int argc, char **argv, char **envp)
{
	t_shell shell;

	(void)argc;
	(void)argv;
	int i = 0;
	while (envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
	init_shell(&shell, envp);
	run_shell(&shell);
	return (shell.last_exit_status);
}
