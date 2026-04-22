#include "minishell.h"

static int find_equal_sign(char *arg)
{
    int i;

    i = 0;
    while (arg[i])
    {
        if (arg[i] == '=')
            return (i);
        i++;
    }
    return (-1);
}

static int env_find_index(char **envp, char *key)
{
    int i;
    int key_len;

    i = 0;
    key_len = ft_strlen(key);
    while (envp[i])
    {
        if (ft_strncmp(envp[i], key, key_len) == 0 && envp[i][key_len] == '=')
            return (i);
        i++;
    }
    return (-1);
}

int env_count(char **envp)
{
    int i;

    i = 0;
    if (!envp)
        return (0);
    while (envp[i])
        i++;
    return (i);
}
static char **env_add_or_replace(char **envp, char *new_entry, char *key)
{
    char **new_env;
    int index;
    int count;
    int i;

    index = env_find_index(envp, key);
    if (index != -1)
    {
        free(envp[index]);
        envp[index] = new_entry;
        return (envp);
    }
    count = env_count(envp);
    new_env = malloc(sizeof(char *) * (count + 2));
    if (!new_env)
    {
        free(new_entry);
        return (NULL);
    }
    i = 0;
    while (i < count)
    {
        new_env[i] = envp[i];
        i++;
    }
    new_env[i] = new_entry;
    new_env[i + 1] = NULL;
    free(envp);
    return (new_env);
}
int ft_export(char **argv, t_shell *shell)
{
    int i;
    int eq_index;
    char *key;
    char *new_entry;
    char **envp_temp;

    i = 1;
    while (argv[i])
    {
        eq_index = find_equal_sign(argv[i]);
        if (eq_index == -1)
        {
            i++;
            continue;
        }
        key = ft_substr(argv[i], 0, eq_index);
        new_entry = ft_strdup(argv[i]);
        envp_temp = env_add_or_replace(shell->envp, new_entry, key);
        if (envp_temp)
            shell->envp = envp_temp;
        free(key);
        i++;
    }
    return (0);
}