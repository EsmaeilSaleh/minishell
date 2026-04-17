#include "minishell.h"

void free_split(char **arr)
{
    int i;

    if (arr == NULL)
        return;
    i = 0;
    while (arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr);
}

void free_redirs(t_redir *redirs)
{
    t_redir *next;

    while (redirs)
    {
        next = redirs->next;
        free(redirs->target);
        free(redirs);
        redirs = next;
    }
}