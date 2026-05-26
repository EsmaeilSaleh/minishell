#include "minishell.h"

static void	heredoc_child_loop(int write_fd, char *delimiter,
	int expand_body, t_shell *shell)
{
	char	*line;
	char	*expanded;

	rl_catch_signals = 1;
	set_signal_handler(SIGINT, SIG_DFL);
	set_signal_handler(SIGQUIT, SIG_IGN);
	while (1)
	{
		line = readline(isatty(STDIN_FILENO) ? "> " : NULL);
		if (line == NULL)
			break ;
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (expand_body)
		{
			expanded = expand_one_word(line, shell);
			if (expanded)
			{
				write(write_fd, expanded, ft_strlen(expanded));
				free(expanded);
			}
		}
		else
			write(write_fd, line, ft_strlen(line));
		write(write_fd, "\n", 1);
		free(line);
	}
	close(write_fd);
	exit(0);
}

static int	wait_heredoc_child(pid_t pid, int pipefd[2])
{
	int	status;

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
	int		pipefd[2];
	pid_t	pid;

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
	t_redir	*redir;

	while (cmds)
	{
		redir = cmds->redirs;
		while (redir)
		{
			if (redir->type == TOK_HEREDOC)
			{
				redir->heredoc_fd = prepare_heredoc(redir->target,
						redir->expand_body, shell);
				if (redir->heredoc_fd < 0)
				{
					if (g_signal_status == SIGINT)
						return (130);
					return (1);
				}
			}
			redir = redir->next;
		}
		cmds = cmds->next;
	}
	return (0);
}
