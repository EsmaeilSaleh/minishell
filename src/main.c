#include "minishell.h"

void *get_path_from_envp(char **envp)
{
	int i;

	i = 0;
	while (envp[i])
	{
		if (strncmp(envp[i], "PATH=", 5) == 0)
			printf("%s\n", envp[i]);
		i++;
	}
	return (NULL);
}

int main(int argc, char **argv, char **envp)
{
	t_shell shell;

	(void)argc;
	(void)argv;
	init_shell(&shell, envp);
	run_shell(&shell);
	int i = 0;
	get_path_from_envp(envp);
	/*while (envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
		*/
	return (shell.last_exit_status);
}
