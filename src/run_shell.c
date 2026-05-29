/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_shell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkpg-md- <dkpg-md-@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 00:17:23 by dkpg-md-          #+#    #+#             */
/*   Updated: 2026/05/28 19:47:52 by dkpg-md-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_tokens(t_shell *shell, char *line, int interactive)
{
	t_token	*tokens;
	t_cmd	*cmds;

//	cmds = parser(tokens);
	tokens = lexer(line);
	if (!tokens)
		return ;
	if (!syntax_check(tokens) || !(cmds = parser(tokens)))
	{
		free_tokens(tokens);
		shell->last_exit_status = 2;
		if (!interactive)
			shell->running = 0;
		return ;
	}
	free_tokens(tokens);
	expand_cmds(cmds, shell);
	execute_cmds(cmds, shell);
	free_cmds(cmds);
}

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
		return (free(cont), NULL);
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

/*
old code

#include "minishell.h"

static char	*append_line(char *line, char *next)
{
	char	*with_newline;
	char	*result;

	with_newline = ft_strjoin(line, "\n");
	free(line);
	if (with_newline == NULL)
		return (NULL);
	result = ft_strjoin(with_newline, next);
	free(with_newline);
	return (result);
}

static void	handle_input(t_shell *shell, char *line, int interactive)
{
	t_token	*tokens;
	t_cmd	*cmds;
	char	*next;

	while (has_unclosed_quote(line))
	{
		next = readline("> ");
		if (next == NULL || g_signal_status == SIGINT)
		{
			free(next);
			free(line);
			return ;
		}
		line = append_line(line, next);
		free(next);
		if (line == NULL)
			return ;
	}
	if (*line != '\0')
		add_history(line);
	tokens = lexer(line);
	if (tokens == NULL)
		return (free(line));
	if (!syntax_check(tokens))
	{
		free_tokens(tokens);
		free(line);
		shell->last_exit_status = 2;
		if (!interactive)
			shell->running = 0;
		return ;
	}
	cmds = parser(tokens);
	free_tokens(tokens);
	if (cmds == NULL)
	{
		free(line);
		shell->last_exit_status = 2;
		if (!interactive)
			shell->running = 0;
		return ;
	}
	expand_cmds(cmds, shell);
	execute_cmds(cmds, shell);
	free_cmds(cmds);
	free(line);
}

void	run_shell(t_shell *shell)
{
	char	*line;
	int		interactive;

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
	}
	free_split(shell->envp);
	rl_clear_history();
}
*/