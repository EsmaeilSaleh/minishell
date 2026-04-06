#include "minishell.h"

void free_tokens(t_token *tokens)
{
    t_token *next;

    while (tokens)
    {
        next = tokens->next;
        free(tokens->value);
        free(tokens);
        tokens = next;
    }
}