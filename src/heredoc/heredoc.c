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