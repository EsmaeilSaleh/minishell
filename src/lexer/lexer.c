#include "minishell.h"

static int handle_word(t_token **tokens, char *line, int *i)
{
    char *value;
    t_token *new_node;

    value = extract_word(line, i);
    if (value == NULL)
        return (0);
    new_node = new_token(value, TOK_WORD);
    if (new_node == NULL)
        return (0);
    add_token_back(tokens, new_node);
    return (1);
}

static int handle_operator(t_token **tokens, char *line, int *i)
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
    add_token_back(tokens, new_node);
    return (1);
}

t_token *lexer(char *line)
{
    int i;
    t_token *tokens;

    tokens = NULL;
    i = 0;
    while (line[i])
    {
        if (is_space(line[i]))
            i++;
        else if (is_operator_char(line[i]))
        {
            if (handle_operator(&tokens, line, &i) == 0)
                return (free_tokens(tokens), NULL);
        }
        else
        {
            if (handle_word(&tokens, line, &i) == 0)
                return (free_tokens(tokens), NULL);
        }
    }
    return (tokens);
}