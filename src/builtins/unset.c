#include "minishell.h"

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

    i = 1;
    if (argv[1] == NULL)
        return (0);
    while (argv[i])
    {
        tmp = env_remove_key(shell->envp, argv[i]);
        if (tmp)
            shell->envp = tmp;
        i++;
    }
    return (0);
}