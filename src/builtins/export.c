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

static int is_name_start(char c)
{
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_');
}

static int is_name_char(char c)
{
    return (is_name_start(c) || (c >= '0' && c <= '9'));
}

static int is_valid_identifier(char *arg, int eq_index)
{
    int i;

    if (arg == NULL || arg[0] == '\0')
        return (0);
    if (eq_index == -1)
        eq_index = ft_strlen(arg);
    if (eq_index == 0 || !is_name_start(arg[0]))
        return (0);
    i = 1;
    while (i < eq_index)
    {
        if (!is_name_char(arg[i]))
            return (0);
        i++;
    }
    return (1);
}

int env_find_index(char **envp, char *key)
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
    int status;

    i = 1;
    status = 0;
    while (argv[i])
    {
        eq_index = find_equal_sign(argv[i]);
        if (!is_valid_identifier(argv[i], eq_index))
        {
            fprintf(stderr, "export: `%s': not a valid identifier\n", argv[i]);
            status = 1;
            i++;
            continue;
        }
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
    return (status);
}
