#include "minishell.h"

int prepare_heredoc(char *delimiter)
{
    int pipefd[2];
    char *line;

    if (pipe(pipefd) == -1)
        return (-1);
    while (1)
    {
        line = readline("> ");
        if (line == NULL)
            break;
        if (g_signal_status == SIGINT)

        {

            if (line)
                free(line);
            close(pipefd[0]);
            close(pipefd[1]);
            setup_signals();
            return (-1);
        }
        if (ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
        write(pipefd[1], line, ft_strlen(line));
        write(pipefd[1], "\n", 1);
        free(line);
    }
    close(pipefd[1]);
    return (pipefd[0]);
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
                    return (1);
            }
            redir = redir->next;
        }
        cmds = cmds->next;
    }
    return (0);
}