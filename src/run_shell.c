#include "minishell.h"

static void handle_input(t_shell *shell, char *line)
{
	t_token *tokens;

	if (*line != '\0')
		add_history(line);
	tokens = lexer(line);
	if (tokens == NULL)
		return;
	print_tokens(tokens);
	if (!syntax_check(tokens))
	{
		free_tokens(tokens);
		shell->last_exit_status = 2;
		return;
	}
	cmds = parser(tokens);
	if (cmds == NULL)
	{
		free_tokens(tokens);
		return;
	}
	print_cmds(cmds);
	free_tokens(tokens);
	free_cmds(cmds);
	shell->last_exit_status = 0;
}

void run_shell(t_shell *shell)
{
	char *line;

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