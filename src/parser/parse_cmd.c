#include "minishell.h"

static void free_parial_argv(char **argv, int count)
{
    while (count > 0)
    {
        count--;
        free(argv[count]);
    }
    free(argv);
}

static t_cmd *parse_cmd_error(char **argv, int i, t_redir *redirs)
{
    free_partial_argv(argv, i);
    free_redirs(redirs);
    return (NULL);
}

t_cmd *parse_command(t_token *tokens)
{
    char **argv;
    t_redir *redirs;
    int argc;
    int i;
    char *target;
    t_redir *new_node;

    argc = count_cmd_args(tokens);
    argv = malloc(sizeof(char *) * (argc + 1));
    if (argv == NULL)
        return (NULL);
    redirs = NULL;
    i = 0;
    while (tokens && tokens->type != TOK_PIPE)
    {
        if (is_redir_token(tokens))
        {
            target = ft_strdup(tokens->next->value);
            if (target == NULL)
                return (cleenup..);
            new_node = new_redir(tokens->type, target);
            if (new_node == NULL)
                return (cleanup...);
            add_redir_back(&redirs, new_node);
            tokens = tokens->next->next;
        }
        else if (tokens->type == TOK_WORD)
        {
            argv[i] = ft_strdup(tokens->value);
            if (argv[i] == NULL)
                return (cleenup...);
            i++;
            tokens = tokens->next;
        }
        else
            tokens = tokens->next;
    }
    argv[i] = NULL;
    return (new_cmd(argv, redirs));
}