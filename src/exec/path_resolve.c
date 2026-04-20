#include "minishell.h"

int has_slash(char *cmd)
{
    int i;

    i = 0;
    while (cmd[i])
    {
        if (cmd[i] == '/')
            return (1);
        i++;
    }
    return (0);
}

char *get_env_value(char **envp, const char *name)
{
    int i;
    size_t len;

    i = 0;
    len = strlen(name);
    while (envp[i])
    {
        if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
            return (envp[i] + len + 1);
        i++;
    }
    return (NULL);
}
char *resolve_command_path(char *cmd_name, char **envp)
{
    char *path_value;
    char **dirs;
    char *full_path;
    int i;

    if (!cmd_name || !*cmd_name)
        return (NULL);
    if (has_slash(cmd_name))
    {
        if (access(cmd_name, X_OK) == 0)
            return (ft_strdup(cmd_name));
        return (NULL);
    }
    path_value = get_env_value(envp, "PATH");
    if (!path_value)
        return (NULL);
    dirs = ft_split(path_value, ':');
    if (!dirs)
        return (NULL);
    i = 0;
    while (dirs[i])
    {
        full_path = ft_join_path(dirs[i], cmd_name);
        if (full_path && access(full_path, X_OK) == 0)
        {
            free_split(dirs);
            return (full_path);
        }
        free(full_path);
        i++;
    }
    free_split(dirs);
    return (NULL);
}