#include "minishell.h"

static int	has_unclosed_quotes(char *line)
{
	int	i;
	int	single_quote;
	int	double_quote;

	i = 0;
	single_quote = 0;
	double_quote = 0;
	while (line[i])
	{
		if (line[i] == '\'' && !double_quote)
			single_quote = !single_quote;
		else if (line[i] == '"' && !single_quote)
			double_quote = !double_quote;
		i++;
	}
	return (single_quote || double_quote);
}

static char	*read_non_interactive_line(void)
{
	char	*line;
	char	c;
	int		len;
	int		capacity;
	int		bytes_read;
	char	*new_line;
	int		i;
	char	*joined_line;

	line = malloc(128);
	if (line == NULL)
		return (NULL);
	len = 0;
	capacity = 128;
	while (1)
	{
		bytes_read = read(STDIN_FILENO, &c, 1);
		if (bytes_read <= 0)
			break ;
		if (c == '\n')
			break ;
		if (len + 1 >= capacity)
		{
			new_line = malloc(capacity * 2);
			if (new_line == NULL)
			{
				free(line);
				return (NULL);
			}
			i = 0;
			while (i < len)
			{
				new_line[i] = line[i];
				i++;
			}
			free(line);
			line = new_line;
			capacity *= 2;
		}
		line[len++] = c;
	}
	if (bytes_read <= 0 && len == 0)
	{
		free(line);
		return (NULL);
	}
	line[len] = '\0';
	while (has_unclosed_quotes(line))
	{
		joined_line = ft_strjoin(line, "\n");
		free(line);
		if (joined_line == NULL)
			return (NULL);
		line = joined_line;
		new_line = read_non_interactive_line();
		if (new_line == NULL)
			break ;
		joined_line = ft_strjoin(line, new_line);
		free(line);
		free(new_line);
		if (joined_line == NULL)
			return (NULL);
		line = joined_line;
	}
	return (line);
}

static void handle_input(t_shell *shell, char *line, int interactive)
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
		if (!interactive)
			shell->running = 0;
		return;
	}
	cmds = parser(tokens);
	if (cmds == NULL)
	{
		free_tokens(tokens);
		shell->last_exit_status = 2;
		if (!interactive)
			shell->running = 0;
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
	char	*line;
	int		interactive;

	rl_catch_signals = 0;
	interactive = isatty(STDIN_FILENO) && isatty(STDOUT_FILENO);
	while (shell->running)
	{
		setup_signals();
		if (interactive)
			line = readline("minishell$ ");
		else
			line = read_non_interactive_line();
		if (line == NULL)
		{
			if (interactive)
				printf("exit\n");
			break;
		}
		handle_input(shell, line, interactive);
		free(line);
	}
}
