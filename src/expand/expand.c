#include "minishell.h"

static char *append_str(char *result, char *to_add)
{
    char *new_str;
    int len1;
    int len2;
    int i;
    int j;

    if (result == NULL || to_add == NULL)
        return (NULL);
    len1 = ft_strlen(result);
    len2 = ft_strlen(to_add);
    new_str = malloc(sizeof(char) * (len1 + len2 + 1));
    if (!new_str)
        return (NULL);
    i = 0;
    j = 0;
    while (result[i])
    {
        new_str[i] = result[i];
        i++;
    }
    while (to_add[j])
    {
        new_str[i] = to_add[j];
        i++;
        j++;
    }
    new_str[i] = '\0';
    free(result);
    return (new_str);
}

static int is_var_char(char c)
{
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_');
}

static char *status_to_str(int status)
{
    char *str;

    str = ft_itoa(status);
    return (str);
}

static char *expand_dollar(char *word, int *i, t_shell *shell)
{
    int start;
    char *name;
    char *value;

    if (word[*i + 1] == '?')
    {
        *i += 2;
        return (status_to_str(shell->last_exit_status));
    }
    start = *i + 1;
    while (word[start] && is_var_char(word[start]))
        start++;
    if (start == *i + 1)
    {
        (*i)++;
        return (ft_strdup("$"));
    }
    name = ft_substr(word, *i + 1, start - (*i + 1));
    if (name == NULL)
        return (NULL);
    value = get_env_value(shell->envp, name);
    free(name);
    *i = start;
    if (value == NULL)
        return (ft_strdup(""));
    return (ft_strdup(value));
}

static char *copy_double_quoted(char *word, int *i, t_shell *shell)
{
    char *result;
    char *piece;
    char tmp[2];

    result = ft_strdup("");
    if (result == NULL)
        return (NULL);
    (*i)++;
    while (word[*i] && word[*i] != '"')
    {
        if (word[*i] == '$')
            piece = expand_dollar(word, i, shell);
        else
        {
            tmp[0] = word[*i];
            tmp[1] = '\0';
            piece = ft_strdup(tmp);
            (*i)++;
        }
        if (piece == NULL)
        {
            free(result);
            return (NULL);
        }
        result = append_str(result, piece);
        free(piece);
        if (result == NULL)
            return (NULL);
    }
    if (word[*i] == '"')
        (*i)++;
    return (result);
}

char *expand_one_word(char *word, t_shell *shell)
{
    char *result;
    char *piece;
    char tmp[2];
    int i;

    if (word == NULL)
        return (NULL);
    result = ft_strdup("");
    if (result == NULL)
        return (NULL);
    i = 0;
    while (word[i])
    {
        if (word[i] == '\'')

            piece = copy_single_quoted(word, &i);
        else if (word[i] == '"')
            piece = copy_double_quoted(word, &i, shell);
        else if (word[i] == '$')
            piece = expand_dollar(word, &i, shell);
        else
        {
            tmp[0] = word[i];
            tmp[1] = '\0';
            piece = ft_strdup(tmp);
            i++;
        }
        if (piece == NULL)
        {
            free(result);
            return (NULL);
        }
        result = append_str(result, piece);
        free(piece);
        if (result == NULL)
            return (NULL);
    }
    return (result);
}

static void expand_argv(char **argv, t_shell *shell)
{
    int i;
    char *new_word;

    if (argv == NULL)
        return;
    i = 0;
    while (argv[i])
    {
        new_word = expand_one_word(argv[i], shell);
        if (new_word != NULL)
        {
            free(argv[i]);
            argv[i] = new_word;
        }
        i++;
    }
}

static void expand_redirs(t_redir *redirs, t_shell *shell)
{
    char *new_target;
    while (redirs)
    {
        new_target = expand_one_word(redirs->target, shell);
        if (new_target != NULL)
        {
            free(redirs->target);
            redirs->target = new_target;
        }
        redirs = redirs->next;
    }
}

void expand_cmds(t_cmd *cmds, t_shell *shell)
{
    while (cmds)
    {
        expand_argv(cmds->argv, shell);
        expand_redirs(cmds->redirs, shell);
        cmds = cmds->next;
    }
}