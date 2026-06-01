/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_shell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkpg-md- <dkpg-md-@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 00:17:23 by dkpg-md-          #+#    #+#             */
/*   Updated: 2026/06/01 16:28:24 by dkpg-md-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_input(t_shell *shell, char *line, int interactive)
{
	int		semi;
	char	*before;
	char	*after;

	semi = find_semi_outside_quotes(line);
	if (semi != -1)
	{
		before = ft_substr(line, 0, semi);
		after = ft_strdup(line + semi + 1);
		if (before)
		{
			handle_input(shell, before, interactive);
			free(before);
		}
		if (after && shell->running)
			handle_input(shell, after, interactive);
		free(after);
		return ;
	}
	if (*line != '\0')
		add_history(line);
	process_tokens(shell, line, interactive);
}

static char	*read_continuation(char *line)
{
	char	*cont;
	char	*tmp;

	cont = readline("> ");
	if (g_signal_status == SIGINT)
	{
		free(cont);
		free(line);
		return (NULL);
	}
	if (cont == NULL)
		return (line);
	tmp = ft_strjoin(line, "\n");
	free(line);
	if (tmp == NULL)
	{
		free(cont);
		return (NULL);
	}
	line = ft_strjoin(tmp, cont);
	free(tmp);
	free(cont);
	return (line);
}

static char	*read_interactive_line(void)
{
	char	*line;

	line = readline("minishell$ ");
	while (line != NULL && has_unclosed_quotes(line))
	{
		line = read_continuation(line);
		if (line == NULL)
		{
			if (g_signal_status == SIGINT)
				return (ft_strdup(""));
			return (NULL);
		}
	}
	return (line);
}

static void	disable_echoctl(void)
{
	struct termios	term;

	if (isatty(STDIN_FILENO) && tcgetattr(STDIN_FILENO, &term) == 0)
	{
		term.c_lflag &= ~ECHOCTL;
		tcsetattr(STDIN_FILENO, TCSANOW, &term);
	}
}

void	run_shell(t_shell *shell)
{
	char	*line;
	int		interactive;

	disable_echoctl();
	interactive = isatty(STDIN_FILENO) && isatty(STDOUT_FILENO);
	while (shell->running)
	{
		if (g_signal_status == SIGINT)
			shell->last_exit_status = 130;
		setup_signals();
		if (interactive)
			line = read_interactive_line();
		else
			line = read_non_interactive_line();
		if (line == NULL)
		{
			if (interactive)
				write(1, "exit\n", 5);
			break ;
		}
		handle_input(shell, line, interactive);
		free(line);
	}
	free_split(shell->envp);
	rl_clear_history();
}
