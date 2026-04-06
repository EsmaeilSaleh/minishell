#include "minishell.h"

t_token *list_last(t_token *list)
{
    if (!list)
        return (NULL);
    while (list->next)
        list = list->next;
    return (list);
}

void add_token_back(t_token **list, t_token *new_node)
{
    t_token *last;

    if (!list || !new_node)
        return;
    if (!*list)
    {
        *list = new_node;
        return;
    }
    last = list_last(*list);
    last->next = new_node;
}