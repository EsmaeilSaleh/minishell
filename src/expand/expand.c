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


static int	append_masked(char **r, char **m, char *add, char mv)
{
    int     rl;
    int     al;
    char    *nr;
    char    *nm;
    int     i;
    int     j;

    rl = ft_strlen(*r);
    al = ft_strlen(add);
    nr = malloc(rl + al + 1);
    nm = malloc(rl + al + 1);
    if (!nr || !nm)
    {
        free(nr);
        free(nm);
        return (-1);
    }
    i = 0;
    while (i < rl)
    {
        nr[i] = (*r)[i];
        nm[i] = (*m)[i];
        i++;
    }
    j = 0;
    while (j < al)
    {
        nr[i] = add[j];
        nm[i] = mv;
        i++;
        j++;
    }
    nr[i] = '\0';
    nm[i] = '\0';
    free(*r);
    free(*m);
    *r = nr;
    *m = nm;
    return (0);
}

static int	expand_dq_masked(char *word, int *i, t_shell *shell,
    char **r, char **m)
{
    char    *piece;
    char    tmp[2];

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
        if (!piece || append_masked(r, m, piece, '1') == -1)
        {
            free(piece);
            return (-1);
        }
        free(piece);
    }
    if (word[*i] == '"')
        (*i)++;
    return (0);
}

static int	expand_sq_masked(char *word, int *i, char **r, char **m)
{
    char    *piece;
    int     start;

    (*i)++;
    start = *i;
    while (word[*i] && word[*i] != '\'')
        (*i)++;
    piece = ft_substr(word, start, *i - start);
    if (!piece || append_masked(r, m, piece, '1') == -1)
    {
        free(piece);
        return (-1);
    }
    free(piece);
    if (word[*i] == '\'')
        (*i)++;
    return (0);
}

static char	*expand_word_masked(char *word, t_shell *shell, char **mask_out)
{
    char    *result;
    char    *mask;
    char    *piece;
    char    tmp[2];
    int     i;
    char    *home;

    *mask_out = NULL;
    if (!word)
        return (NULL);
    result = ft_strdup("");
    mask = ft_strdup("");
    if (!result || !mask)
    {
        free(result);
        free(mask);
        return (NULL);
    }
    i = 0;
    if (word[0] == '~' && (word[1] == '\0' || word[1] == '/'))
    {
        home = get_env_value(shell->envp, "HOME");
        if (home && append_masked(&result, &mask, home, '0') == -1)
        {
            free(result);
            free(mask);
            return (NULL);
        }
        if (home)
            i = 1;
    }
    while (word[i])
    {
        piece = NULL;
        if (word[i] == '\\' && word[i + 1] != '\0')
        {
            tmp[0] = word[i + 1];
            tmp[1] = '\0';
            piece = ft_strdup(tmp);
            i += 2;
            if (!piece || append_masked(&result, &mask, piece, '0') == -1)
            {
                free(piece);
                free(result);
                free(mask);
                return (NULL);
            }
            free(piece);
        }
        else if (word[i] == '\\')
        {
            tmp[0] = '\\';
            tmp[1] = '\0';
            if (append_masked(&result, &mask, tmp, '0') == -1)
            {
                free(result);
                free(mask);
                return (NULL);
            }
            i++;
        }
        else if (word[i] == '$' && word[i + 1] == '\'')
        {
            i++;
            if (expand_sq_masked(word, &i, &result, &mask) == -1)
            {
                free(result);
                free(mask);
                return (NULL);
            }
        }
        else if (word[i] == '$' && word[i + 1] == '"')
        {
            i++;
            if (expand_dq_masked(word, &i, shell, &result, &mask) == -1)
            {
                free(result);
                free(mask);
                return (NULL);
            }
        }
        else if (word[i] == '\'')
        {
            if (expand_sq_masked(word, &i, &result, &mask) == -1)
            {
                free(result);
                free(mask);
                return (NULL);
            }
        }
        else if (word[i] == '"')
        {
            if (expand_dq_masked(word, &i, shell, &result, &mask) == -1)
            {
                free(result);
                free(mask);
                return (NULL);
            }
        }
        else if (word[i] == '$')
        {
            piece = expand_dollar(word, &i, shell);
            if (!piece || append_masked(&result, &mask, piece, '0') == -1)
            {
                free(piece);
                free(result);
                free(mask);
                return (NULL);
            }
            free(piece);
        }
        else
        {
            tmp[0] = word[i];
            tmp[1] = '\0';
            if (append_masked(&result, &mask, tmp, '0') == -1)
            {
                free(result);
                free(mask);
                return (NULL);
            }
            i++;
        }
    }
    *mask_out = mask;
    return (result);
}

static char	**split_masked(char *word, char *mask)
{
    char    **fields;
    int     count;
    int     i;
    int     start;
    int     field_i;

    count = 0;
    i = 0;
    while (word[i])
    {
        while (word[i] && mask[i] == '0' && is_space(word[i]))
            i++;
        if (!word[i])
            break ;
        count++;
        while (word[i] && !(mask[i] == '0' && is_space(word[i])))
            i++;
    }
    if (count == 0)
        return (NULL);
    fields = malloc(sizeof(char *) * (count + 1));
    if (!fields)
        return (NULL);
    i = 0;
    field_i = 0;
    while (word[i])
    {
        while (word[i] && mask[i] == '0' && is_space(word[i]))
            i++;
        if (!word[i])
            break ;
        start = i;
        while (word[i] && !(mask[i] == '0' && is_space(word[i])))
            i++;
        fields[field_i] = ft_substr(word, start, i - start);
        if (!fields[field_i])
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
    int     i;
    int     count;
    int     has_quotes;
    char    *new_word;
    char    *mask;
    char    **new_argv;
    char    **split_words;
    int     split_i;

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
        mask = NULL;
        new_word = expand_word_masked(argv[i], shell, &mask);
        free(argv[i]);
        if (new_word != NULL)
        {
            if (new_word[0] == '\0')
            {
                if (has_quotes)
                    new_argv = append_arg(new_argv, &count, new_word);
                else
                    free(new_word);
                free(mask);
            }
            else if (mask != NULL)
            {
                split_words = split_masked(new_word, mask);
                free(mask);
                free(new_word);
                if (split_words != NULL)
                {
                    split_i = 0;
                    while (split_words[split_i])
                        new_argv = append_arg(new_argv, &count,
                            split_words[split_i++]);
                    free(split_words);
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

char	*expand_heredoc_body_line(char *line, t_shell *shell)
{
	char	*result;
	char	*piece;
	char	tmp[3];
	int		i;

	if (line == NULL)
		return (NULL);
	result = ft_strdup("");
	if (result == NULL)
		return (NULL);
	i = 0;
	while (line[i])
	{
		if (line[i] == '$')
			piece = expand_dollar(line, &i, shell);
		else if (line[i] == '\\')
		{
			if (line[i + 1] == '\\' || line[i + 1] == '$' || line[i + 1] == '`')
			{
				tmp[0] = line[i + 1];
				tmp[1] = '\0';
				piece = ft_strdup(tmp);
				i += 2;
			}
			else if (line[i + 1] != '\0')
			{
				tmp[0] = '\\';
				tmp[1] = line[i + 1];
				tmp[2] = '\0';
				piece = ft_strdup(tmp);
				i += 2;
			}
			else
			{
				tmp[0] = '\\';
				tmp[1] = '\0';
				piece = ft_strdup(tmp);
				i++;
			}
		}
		else
		{
			tmp[0] = line[i];
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

void expand_cmds(t_cmd *cmds, t_shell *shell)
{
    while (cmds)
    {
        cmds->argv = expand_argv(cmds->argv, shell);
        expand_redirs(cmds->redirs, shell);
        cmds = cmds->next;
    }
}
