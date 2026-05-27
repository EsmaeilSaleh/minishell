#include "minishell.h"

static int is_digit_str(char *s)
{
    int i;

    if (!s || !s[0])
        return (0);
    i = 0;
    while (s[i])
    {
        if (s[i] < '0' || s[i] > '9')
            return (0);
        i++;
    }
    return (1);
}

static int handle_word(t_token **tokens, char *line, int *i, int *pending_fd)
{
    char *value;
    t_token *new_node;

    value = extract_word(line, i);
    if (value == NULL)
        return (0);
    if (is_digit_str(value) && is_operator_char(line[*i]))
    {
        *pending_fd = ft_atoi(value);
        free(value);
        return (1);
    }
    *pending_fd = -1;
    new_node = new_token(value, TOK_WORD);
    if (new_node == NULL)
        return (0);
    add_token_back(tokens, new_node);
    return (1);
}

static int handle_operator(t_token **tokens, char *line, int *i, int pending_fd)
{
    char *value;
    t_token_type type;
    t_token *new_node;

    if (line[*i] == '|')
    {
        value = ft_strdup("|");
        type = TOK_PIPE;
        (*i)++;
    }
    else if (line[*i] == '<' && line[*i + 1] == '<')
    {
        value = ft_strdup("<<");
        type = TOK_HEREDOC;
        (*i) += 2;
    }
    else if (line[*i] == '<')
    {
        value = ft_strdup("<");
        type = TOK_REDIR_IN;
        (*i)++;
    }
    else if (line[*i] == '>' && line[*i + 1] == '>')
    {
        value = ft_strdup(">>");
        type = TOK_APPEND;
        (*i) += 2;
    }
    else if (line[*i] == '>' && line[*i + 1] == '|')
    {
        value = ft_strdup(">");
        type = TOK_REDIR_OUT;
        (*i) += 2;
    }
    else
    {
        value = ft_strdup(">");
        type = TOK_REDIR_OUT;
        (*i)++;
    }
    if (value == NULL)
        return (0);
    new_node = new_token(value, type);
    if (new_node == NULL)
        return (0);
    if (pending_fd != -1 && type != TOK_PIPE)
        new_node->fd = pending_fd;
    add_token_back(tokens, new_node);
    return (1);
}

t_token *lexer(char *line)
{
    int i;
    t_token *tokens;
    int pending_fd;

    tokens = NULL;
    i = 0;
    pending_fd = -1;
    while (line[i])
    {
        if (is_space(line[i]))
        {
            pending_fd = -1;
            i++;
        }
        else if (is_operator_char(line[i]))
        {
            if (handle_operator(&tokens, line, &i, pending_fd) == 0)
                return (free_tokens(tokens), NULL);
            pending_fd = -1;
        }
        else
        {
            if (handle_word(&tokens, line, &i, &pending_fd) == 0)
                return (free_tokens(tokens), NULL);
        }
    }
    return (tokens);
}
