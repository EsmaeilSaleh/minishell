#include "minishell.h"

static char *token_type_name(t_token_type type)
{
    if (type == TOK_WORD)
        return ("WORD");
    if (type == TOK_PIPE)
        return ("PIPE");
    if (type == TOK_REDIR_IN)
        return ("REDIR_IN");
    if (type == TOK_REDIR_OUT)
        return ("REDIR_OUT");
    if (type == TOK_APPEND)
        return ("APPEND");
    if (type == TOK_HEREDOC)
        return ("HEREDOC");
    return ("UNKNOWN");
}

void print_tokens(t_token *tokens)
{
    while (tokens)
    {
        printf("[%s: %s]\n", token_type_name(tokens->type), tokens->value);
        tokens = tokens->next;
    }
}