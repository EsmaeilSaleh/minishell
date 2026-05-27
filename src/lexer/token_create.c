#include "minishell.h"

t_token *new_token(char *value, t_token_type type)
{
    t_token *new;

    new = (t_token *)malloc(sizeof(t_token));
    if (!new)
        return (NULL);
    new->value = value;
    new->type = type;
    new->fd = -1;
    new->next = NULL;
    return (new);
}