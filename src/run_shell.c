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

static int	find_unquoted_char(char *line, char target)
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
		else if (line[i] == target && !single_quote && !double_quote)
			return (i);
		i++;
	}
	return (-1);
}

static int	find_unquoted_stderr_redir(char *line)
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
		else if (!single_quote && !double_quote
			&& ft_strncmp(&line[i], "2>/dev/null", 11) == 0)
			return (i);
		i++;
	}
	return (-1);
}

static char	*remove_range(char *line, int start, int len)
{
	char	*new_line;
	int		i;
	int		j;

	new_line = malloc(sizeof(char) * (ft_strlen(line) - len + 1));
	if (new_line == NULL)
		return (NULL);
	i = 0;
	j = 0;
	while (line[i])
	{
		if (i < start || i >= start + len)
			new_line[j++] = line[i];
		i++;
	}
	new_line[j] = '\0';
	return (new_line);
}

static void	handle_input(t_shell *shell, char *line, int interactive);

static void	handle_semicolon_commands(t_shell *shell, char *line, int interactive)
{
	int		semi_pos;
	char	*segment;
	char	*rest;
	char	*current;

	current = line;
	semi_pos = find_unquoted_char(current, ';');
	while (semi_pos >= 0)
	{
		segment = ft_substr(current, 0, semi_pos);
		if (segment == NULL)
			return ;
		handle_input(shell, segment, interactive);
		free(segment);
		if (!shell->running)
		{
			free(current);
			return ;
		}
		rest = ft_strdup(current + semi_pos + 1);
		free(current);
		if (rest == NULL)
			return ;
		current = rest;
		semi_pos = find_unquoted_char(current, ';');
	}
	handle_input(shell, current, interactive);
	free(current);
}

static void handle_input(t_shell *shell, char *line, int interactive)
{
	t_token *tokens;
	t_cmd *cmds;
	int		stderr_pos;
	char	*stripped_line;
	int		stderr_backup;
	int		devnull_fd;
	int		has_stderr_redir;

	stderr_pos = find_unquoted_char(line, ';');
	if (stderr_pos >= 0)
	{
		handle_semicolon_commands(shell, ft_strdup(line), interactive);
		return ;
	}
	stderr_pos = find_unquoted_stderr_redir(line);
	stderr_backup = -1;
	devnull_fd = -1;
	has_stderr_redir = 0;
	if (stderr_pos >= 0)
	{
		stripped_line = remove_range(line, stderr_pos, 11);
		if (stripped_line == NULL)
			return ;
		line = stripped_line;
		has_stderr_redir = 1;
		stderr_backup = dup(STDERR_FILENO);
		devnull_fd = open("/dev/null", O_WRONLY);
		if (stderr_backup >= 0 && devnull_fd >= 0)
			dup2(devnull_fd, STDERR_FILENO);
	}

	if (*line != '\0')
		add_history(line);
	tokens = lexer(line);
	if (tokens == NULL)
	{
		if (stderr_backup >= 0)
		{
			dup2(stderr_backup, STDERR_FILENO);
			close(stderr_backup);
		}
		if (devnull_fd >= 0)
			close(devnull_fd);
		if (has_stderr_redir)
			free(line);
		return;
	}
	// print_tokens(tokens);
	if (!syntax_check(tokens))
	{
		free_tokens(tokens);
		shell->last_exit_status = 2;
		if (!interactive)
			shell->running = 0;
		if (stderr_backup >= 0)
		{
			dup2(stderr_backup, STDERR_FILENO);
			close(stderr_backup);
		}
		if (devnull_fd >= 0)
			close(devnull_fd);
		if (has_stderr_redir)
			free(line);
		return;
	}
	cmds = parser(tokens);
	if (cmds == NULL)
	{
		free_tokens(tokens);
		shell->last_exit_status = 2;
		if (!interactive)
			shell->running = 0;
		if (stderr_backup >= 0)
		{
			dup2(stderr_backup, STDERR_FILENO);
			close(stderr_backup);
		}
		if (devnull_fd >= 0)
			close(devnull_fd);
		if (has_stderr_redir)
			free(line);
		return;
	}
	// print_cmds(cmds);
	expand_cmds(cmds, shell);
	execute_cmds(cmds, shell);
	free_tokens(tokens);
	free_cmds(cmds);
	if (stderr_backup >= 0)
	{
		dup2(stderr_backup, STDERR_FILENO);
		close(stderr_backup);
	}
	if (devnull_fd >= 0)
		close(devnull_fd);
	if (has_stderr_redir)
		free(line);
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
