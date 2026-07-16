/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esaleh <esaleh@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 10:39:54 by esaleh            #+#    #+#             */
/*   Updated: 2026/07/16 10:39:55 by esaleh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void write_heredoc_line(int write_fd, char *line, int expand_body,
							   t_shell *shell)
{
	char *expanded;

	if (expand_body)
	{
		expanded = expand_heredoc_body_line(line, shell);
		if (expanded)
		{
			write(write_fd, expanded, ft_strlen(expanded));
			free(expanded);
		}
	}
	else
		write(write_fd, line, ft_strlen(line));
	write(write_fd, "\n", 1);
}

static void heredoc_child_loop(int write_fd, char *delimiter,
							   int expand_body, t_shell *shell)
{
	char *line;

	rl_catch_signals = 1;
	set_signal_handler(SIGINT, SIG_DFL);
	set_signal_handler(SIGQUIT, SIG_IGN);
	while (1)
	{
		if (isatty(STDIN_FILENO))
			line = readline("> ");
		else
			line = read_line_fd(STDIN_FILENO);
		if (line == NULL)
		{
			ft_putstr_fd("bash: warning: here-document at line 1 delimited "
						 "by end-of-file (wanted `",
						 2);
			ft_putstr_fd(delimiter, 2);
			ft_putstr_fd("')\n", 2);
			break;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		}
		write_heredoc_line(write_fd, line, expand_body, shell);
		free(line);
	}
	close(write_fd);
	exit(0);
}

static int wait_heredoc_child(pid_t pid, int pipefd[2])
{
	int status;

	close(pipefd[1]);
	set_signal_handler(SIGINT, SIG_IGN);
	set_signal_handler(SIGQUIT, SIG_IGN);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		close(pipefd[0]);
		return (-1);
	}
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		g_signal_status = SIGINT;
		close(pipefd[0]);
		return (-1);
	}
	if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
	{
		close(pipefd[0]);
		return (-1);
	}
	return (pipefd[0]);
}

int prepare_heredoc(char *delimiter, int expand_body, t_shell *shell)
{
	int pipefd[2];
	pid_t pid;

	if (pipe(pipefd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	if (pid == 0)
	{
		close(pipefd[0]);
		heredoc_child_loop(pipefd[1], delimiter, expand_body, shell);
	}
	return (wait_heredoc_child(pid, pipefd));
}

int prepare_heredocs(t_cmd *cmds, t_shell *shell)
{
	int ret;
	t_cmd *head;

	head = cmds;
	while (cmds)
	{
		ret = process_redirs(cmds->redirs, head, shell);
		if (ret != 0)
			return (ret);
		cmds = cmds->next;
	}
	return (0);
}
