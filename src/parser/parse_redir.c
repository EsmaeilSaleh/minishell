#include "minishell.h"

t_redir *new_redir(t_token_type type, char *target)
{
    t_redir *node;

    node = malloc(sizeof(t_redir));
    if (node == NULL)
    {
        free(target);
        return (NULL);
    }
    node->type = type;
    node->target = target;
    node->next = NULL;
    return (node);
}

void add_redir_back(t_redir **list, t_redir *new_node)
{
    t_redir *current;

    if (list == NULL || new_node == NULL)
        return;
    if (*list == NULL)
    {
        *list = new_node;
        return;
    }
    current = *list;
    while (current->next)
        current = current->next;
    current->next = new_node;
}