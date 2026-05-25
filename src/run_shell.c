#include "minishell.h"

static void handle_input(t_shell *shell, char *line)
{
	t_token *tokens;
	t_cmd *cmds;

	if (*line != '\0')
		add_history(line);
	tokens = lexer(line);
	if (tokens == NULL)
		return;
	// print_tokens(tokens);
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
	// print_cmds(cmds);
	expand_cmds(cmds, shell);
	execute_cmds(cmds, shell);
	free_tokens(tokens);
	free_cmds(cmds);
	// shell->last_exit_status = 0;
}

void run_shell(t_shell *shell)
{
	char *line;

	rl_catch_signals = 0;
	while (shell->running)
	{
		setup_signals();
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
