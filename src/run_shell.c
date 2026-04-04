#include "minishell.h"

static void	handle_input(t_shell *shell, char *line)
{
	if (*line != '\0')
		add_history(line);
	printf("You entered: %s\n", line);
	shell->last_exit_status = 0;
}

void	run_shell(t_shell *shell)
{
	char	*line;

	setup_signals();
	while (shell->running)
	{
		line = readline("minishell$ ");
		if (line == NULL)
		{
			printf("exit\n");
			break;
		}
		handle_input(shell, line);
		free(line);
	}
}
