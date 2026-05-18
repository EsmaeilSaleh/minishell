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

char *expand_one_word(char *word, t_shell *shell)
{
    char *result;
    char tmp[2];
    int i;

    (void)shell;
    if (word == NULL)
        return (NULL);
    result = ft_strdup("");
    if (result == NULL)
        return (NULL);
    i = 0;
    while (word[i])
    {
        tmp[0] = word[i];
        tmp[1] = '\0';
        result = append_str(result, tmp);
        if (result == NULL)
            return (NULL);
        i++;
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