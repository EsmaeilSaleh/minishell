#include "minishell.h"

static void	heredoc_child_loop(int write_fd, char *delimiter)
{
	char	*line;

	setup_heredoc_signals();
	while (1)
	{
		line = readline("> ");
		if (g_signal_status == SIGINT)
		{
			if (line)
				free(line);
			close(write_fd);
			exit(130);
		}
		if (line == NULL)
			break ;
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
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
	setup_signals();
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
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

int prepare_heredoc(char *delimiter)
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
        heredoc_child_loop(pipefd[1], delimiter);
    }
    return (wait_heredoc_child(pid, pipefd));
}

int prepare_heredocs(t_cmd *cmds)
{
    t_redir *redir;

    while (cmds)
    {
        redir = cmds->redirs;
        while (redir)
        {
            if (redir->type == TOK_HEREDOC)
            {
                redir->heredoc_fd = prepare_heredoc(redir->target);
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
