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

static char *copy_single_quoted(char *word, int *i)
{
    char *piece;
    int start;

    (*i)++;
    start = *i;
    while (word[*i] && word[*i] != '\'')
        (*i)++;
    piece = ft_substr(word, start, *i - start);
    if (word[*i] == '\'')
        (*i)++;
    return (piece);
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
    char *home;

    if (word == NULL)
        return (NULL);
    if (word[0] == '~' && (word[1] == '\0' || word[1] == '/'))
    {
        home = get_env_value(shell->envp, "HOME");
        if (home != NULL)
        {
            result = ft_strdup(home);
            if (result == NULL)
                return (NULL);
            i = 1;
        }
        else
        {
            result = ft_strdup("");
            if (result == NULL)
                return (NULL);
            i = 0;
        }
    }
    else
    {
        result = ft_strdup("");
        if (result == NULL)
            return (NULL);
        i = 0;
    }
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
    int j;
    int argc;
    char *new_word;
    char **new_argv;
    char **split_words;
    int split_i;
    int split_count;
    int has_quotes;

    if (argv == NULL)
        return;
    argc = 0;
    while (argv[argc])
        argc++;
    new_argv = malloc(sizeof(char *) * (argc * 8 + 1));
    if (new_argv == NULL)
        return;
    i = 0;
    j = 0;
    while (argv[i])
    {
        has_quotes = 0;
        split_i = 0;
        while (argv[i][split_i])
        {
            if (argv[i][split_i] == '\'' || argv[i][split_i] == '"')
            {
                has_quotes = 1;
                break ;
            }
            split_i++;
        }
        new_word = expand_one_word(argv[i], shell);
        free(argv[i]);
        if (new_word != NULL)
        {
            if (!has_quotes && new_word[0] == '\0')
                free(new_word);
            else if (!has_quotes)
            {
                split_words = ft_split(new_word, ' ');
                if (split_words == NULL)
                    new_argv[j++] = new_word;
                else
                {
                    split_count = 0;
                    while (split_words[split_count])
                    {
                        if (split_words[split_count][0] != '\0')
                            new_argv[j++] = split_words[split_count];
                        else
                            free(split_words[split_count]);
                        split_count++;
                    }
                    free(split_words);
                    free(new_word);
                }
            }
            else
                new_argv[j++] = new_word;
        }
        i++;
    }
    new_argv[j] = NULL;
    i = 0;
    while (new_argv[i])
    {
        argv[i] = new_argv[i];
        i++;
    }
    argv[i] = NULL;
    free(new_argv);
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
