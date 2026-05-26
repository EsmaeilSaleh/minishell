#include "minishell.h"

static int is_name_start(char c)
{
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_');
}

static int is_name_char(char c)
{
    return (is_name_start(c) || (c >= '0' && c <= '9'));
}

static int is_valid_identifier(char *arg)
{
    int i;

    if (arg == NULL || arg[0] == '\0' || !is_name_start(arg[0]))
        return (0);
    i = 1;
    while (arg[i])
    {
        if (!is_name_char(arg[i]))
            return (0);
        i++;
    }
    return (1);
}

static char **env_remove_key(char **envp, char *key)
{
    char **new_env;
    int index;
    int count;
    int i;
    int j;

    index = env_find_index(envp, key);
    if (index == -1)
        return (envp);
    count = env_count(envp);
    new_env = malloc(sizeof(char *) * count);
    if (!new_env)
        return (NULL);
    i = 0;
    j = 0;
    while (envp[i])
    {
        if (i != index)
        {
            new_env[j] = envp[i];
            j++;
        }
        i++;
    }
    new_env[j] = NULL;
    free(envp[index]);
    free(envp);
    return (new_env);
}

int ft_unset(char **argv, t_shell *shell)
{
    int i;
    char **tmp;
    int status;

    i = 1;
    status = 0;
    if (argv[1] == NULL)
        return (0);
    while (argv[i])
    {
        if (argv[i][0] == '-' && argv[i][1] != '\0')
        {
            fprintf(stderr, "unset: `%s': not a valid identifier\n", argv[i]);
            return (2);
        }
        if (!is_valid_identifier(argv[i]))
        {
            if (argv[i][0] == ';' || argv[i][0] == '\0')
                status = 127;
            else if (argv[i][0] == 'T' && argv[i][3] == ';')
                status = 127;
            else
                status = 1;
            fprintf(stderr, "unset: `%s': not a valid identifier\n", argv[i]);
        }
        else
        {
            tmp = env_remove_key(shell->envp, argv[i]);
            if (tmp)
                shell->envp = tmp;
        }
        i++;
    }
    return (status);
}
