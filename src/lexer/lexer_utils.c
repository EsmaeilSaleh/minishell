#include "minishell.h"

int is_space(char c)
{
    return (c == ' ' || (c >= 9 && c <= 13));
}

int is_operator_char(char c)
{
    return (c == '|' || c == '>' || c == '<');
}

char *extract_word(char *line, int *i)
{
    int start;

    start = *i;
    while (line[*i] && !is_space(line[*i] && !is_operator_char(line[*i])))
        i++;
    return (ft_substr(line, start, *i - start));
}