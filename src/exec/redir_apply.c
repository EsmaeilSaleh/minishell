#include "minishell.h"

void restore_stdio(int stdin_backup, int stdout_backup)
{
    dup2(stdin_backup, STDIN_FILENO);
    dup2(stdout_backup, STDOUT_FILENO);
    close(stdin_backup);
    close(stdout_backup);
}

static int open_redir_file(t_redir *redir)
{
    if (redir->type == TOK_REDIR_IN)
        return (open(redir->target, O_RDONLY));
    if (redir->type == TOK_REDIR_OUT)
        return (open(redir->target, O_WRONLY | O_CREAT | O_TRUNC, 0644));
    if (redir->type == TOK_APPEND)
        return (open(redir->target, O_WRONLY | O_CREAT | O_APPEND, 0644));
    if (redir->type == TOK_HEREDOC)
        return (prepare_heredoc(redir->target));
    return (-1);
}

int apply_redirs(t_redir *redirs)
{
    int fd;
    while (redirs)
    {
        fd = open_redir_file(redirs);
        if (fd < 0)
        {
            perror("redir");
            return (1);
        }
        if ((redirs->type == TOK_REDIR_IN || redirs->type == TOK_HEREDOC) && dup2(fd, STDIN_FILENO) < 0)
        {
            perror("dup2");
            close(fd);
            return (1);
        }

        if ((redirs->type == TOK_REDIR_OUT || redirs->type == TOK_APPEND) && dup2(fd, STDOUT_FILENO) < 0)
        {
            perror("dup2");
            close(fd);
            return (1);
        }
        close(fd);
        redirs = redirs->next;
    }
    return (0);
}