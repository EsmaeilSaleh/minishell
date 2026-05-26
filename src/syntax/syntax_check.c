#include "minishell.h"

static int check_pipe_syntax(t_token *tokens)
{
    if (is_pipe_token(tokens))
        return (0);
    while (tokens)
    {
        if (is_pipe_token(tokens))
        {
            if (tokens->next == NULL)
                return (0);
            if (is_pipe_token(tokens->next))
                return (0);
        }
        tokens = tokens->next;
    }
    return (1);
}

static int check_redir_syntax(t_token *tokens)
{
    while (tokens)
    {
        if (is_redir_token(tokens))
        {
            if (tokens->next == NULL)
                return (0);
            if (tokens->next->type != TOK_WORD)
                return (0);
        }
        tokens = tokens->next;
    }
    return (1);
}

int syntax_check(t_token *token)
{
    if (token == NULL)
        return (1);
    if (!check_pipe_syntax(token))
        return (syntax_error());
    if (!check_redir_syntax(token))
        return (syntax_error());
    return (1);
}
