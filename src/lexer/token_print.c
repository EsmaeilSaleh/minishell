#include "minishell.h"

static char *token_type_name(t_token_type type)
{
}

void print_tokens(t_token *tokens)
{
    while (tokens)
    {
        printf("[%s: %s]\n", token_type_name(tokens->type), tokens->value);
        tokens = tokens->next;
    }
}