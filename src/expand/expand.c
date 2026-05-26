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
        if (word[i] == '\\' && word[i + 1] != '\0')
        {
            tmp[0] = word[i + 1];
            tmp[1] = '\0';
            piece = ft_strdup(tmp);
            i += 2;
        }
        else if (word[i] == '\\')
        {
            tmp[0] = '\\';
            tmp[1] = '\0';
            piece = ft_strdup(tmp);
            i++;
        }
        else if (word[i] == '$' && word[i + 1] == '\'')
        {
            i++;
            piece = copy_single_quoted(word, &i);
        }
        else if (word[i] == '$' && word[i + 1] == '"')
        {
            i++;
            piece = copy_double_quoted(word, &i, shell);
        }
        else if (word[i] == '\'')
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

static int has_quotes_in_raw(char *word)
{
    int i;

    i = 0;
    while (word[i])
    {
        if (word[i] == '\'' || word[i] == '"')
            return (1);
        i++;
    }
    return (0);
}

static char **append_arg(char **argv, int *count, char *value)
{
    char **new_argv;
    int i;

    new_argv = malloc(sizeof(char *) * (*count + 2));
    if (new_argv == NULL)
        return (NULL);
    i = 0;
    while (i < *count)
    {
        new_argv[i] = argv[i];
        i++;
    }
    new_argv[i] = value;
    new_argv[i + 1] = NULL;
    free(argv);
    (*count)++;
    return (new_argv);
}

static char **split_fields(char *word)
{
    char **fields;
    int count;
    int i;
    int start;
    int field_i;

    count = 0;
    i = 0;
    while (word[i])
    {
        while (word[i] && is_space(word[i]))
            i++;
        if (!word[i])
            break;
        count++;
        while (word[i] && !is_space(word[i]))
            i++;
    }
    fields = malloc(sizeof(char *) * (count + 1));
    if (fields == NULL)
        return (NULL);
    i = 0;
    field_i = 0;
    while (word[i])
    {
        while (word[i] && is_space(word[i]))
            i++;
        if (!word[i])
            break;
        start = i;
        while (word[i] && !is_space(word[i]))
            i++;
        fields[field_i] = ft_substr(word, start, i - start);
        if (fields[field_i] == NULL)
        {
            while (field_i > 0)
                free(fields[--field_i]);
            free(fields);
            return (NULL);
        }
        field_i++;
    }
    fields[field_i] = NULL;
    return (fields);
}

static char **expand_argv(char **argv, t_shell *shell)
{
    int i;
    int count;
    int has_quotes;
    char *new_word;
    char **new_argv;
    char **split_words;
    int split_i;

    if (argv == NULL)
        return (NULL);
    new_argv = malloc(sizeof(char *));
    if (new_argv == NULL)
        return (argv);
    new_argv[0] = NULL;
    count = 0;
    i = 0;
    while (argv[i])
    {
        has_quotes = has_quotes_in_raw(argv[i]);
        new_word = expand_one_word(argv[i], shell);
        free(argv[i]);
        if (new_word != NULL)
        {
            if (!has_quotes && new_word[0] == '\0')
                free(new_word);
            else if (!has_quotes)
            {
                split_words = split_fields(new_word);
                if (split_words == NULL)
                    new_argv = append_arg(new_argv, &count, new_word);
                else
                {
                    split_i = 0;
                    while (split_words[split_i])
                    {
                        if (split_words[split_i][0] != '\0')
                            new_argv = append_arg(new_argv, &count, split_words[split_i]);
                        else
                            free(split_words[split_i]);
                        split_i++;
                    }
                    free(split_words);
                    free(new_word);
                }
            }
            else
                new_argv = append_arg(new_argv, &count, new_word);
        }
        i++;
    }
    free(argv);
    return (new_argv);
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
        cmds->argv = expand_argv(cmds->argv, shell);
        expand_redirs(cmds->redirs, shell);
        cmds = cmds->next;
    }
}
