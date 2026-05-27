#include "minishell.h"

static int token_has_quotes(const char *s)
{
    int i;

    i = 0;
    while (s[i])
    {
        if (s[i] == '\'' || s[i] == '"')
            return (1);
        i++;
    }
    return (0);
}

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

static void free_partial_argv(char **argv, int count)
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
                return (parse_cmd_error(argv, i, redirs));
            new_node = new_redir(tokens->type, target);
            if (new_node == NULL)
                return (parse_cmd_error(argv, i, redirs));
            if (tokens->fd != -1)
                new_node->fd = tokens->fd;
            if (tokens->type == TOK_HEREDOC)
                new_node->expand_body = !token_has_quotes(tokens->next->value);
            add_redir_back(&redirs, new_node);
            tokens = tokens->next->next;
        }
        else if (tokens->type == TOK_WORD)
        {
            argv[i] = ft_strdup(tokens->value);
            if (argv[i] == NULL)
                return (parse_cmd_error(argv, i, redirs));
            i++;
            tokens = tokens->next;
        }
        else
            tokens = tokens->next;
    }
    argv[i] = NULL;
    return (new_cmd(argv, redirs));
}