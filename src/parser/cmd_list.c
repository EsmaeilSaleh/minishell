#include "minishell.h"

t_cmd *new_cmd(char **argv, t_redir *redirs)
{
    t_cmd *node;

    node = malloc(sizeof(t_cmd));
    if (!node)
    {
        free_split(argv);
        free_redirs(redirs);
        return (NULL);
    }
    node->argv = argv;
    node->redirs = redirs;
    node->next = NULL;
    return (node);
}

void add_cmd_back(t_cmd **list, t_cmd *new_node)
{
    t_cmd *current;

    if (!list || !new_node)
        return;
    if (!*list)
    {
        *list = new_node;
        return;
    }
    current = *list;
    while (current->next)
        current = current->next;
    current->next = new_node;
}