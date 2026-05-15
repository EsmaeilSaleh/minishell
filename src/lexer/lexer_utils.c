#include "minishell.h"

int is_space(char c)
{
    return (c == ' ' || (c >= 9 && c <= 13));
}

int is_operator_char(char c)
{
    return (c == '|' || c == '>' || c == '<');
}

static void skip_quoted_section(char *line, int *i, char quote)
{
    (*i)++;
    while (line[*i] && line[*i] != quote)
        (*i)++;
    if (line[*i] == quote)
        (*i)++;
}

char *extract_word(char *line, int *i)
{
    int start;

    start = *i;
    while (line[*i])
    {
        if (line[*i] == '\'' || line[*i] == '"')
            skip_quoted_section(line, i, line[*i]);
        else if (is_space(line[*i]) || is_operator_char(line[*i]))
            break;
        else
            (*i)++;
    }
    return (ft_substr(line, start, *i - start));
}