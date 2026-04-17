#include "minishell.h"

int count_cmd_args(t_token *tokens)
{
    int count;

    count = 0;
    while (tokens && tokens->type != TOK_PIPE)
    {
        if (is_redir_token(tokens))
            tokens = tokens->next->next;
        else if (tokens->type == TOK_WORD)
        {
            count++;
            tokens = tokens->next;
        }
        else
            tokens = tokens->next;
    }
    return (count);
}

t_cmd *parser(t_token *tokens)
{
    t_cmd *cmds;
    t_cmd *new_node;

    cmds = NULL;
    while (tokens)
    {
        new_node = parse_command(tokens);
        if (new_node == NULL)
        {
            free_cmds(cmds);
            return (NULL);
        }
        add_cmd_back(&cmds, new_node);
        while (tokens && tokens->type != TOK_PIPE)
            tokens = tokens->next;
        if (tokens && tokens->type == TOK_PIPE)
            tokens = tokens->next;
    }
    return (cmds);
}