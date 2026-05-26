#include "minishell.h"
#include <string.h>
#include <sys/stat.h>

static int	is_directory(char *path)
{
	struct stat	st;

	if (stat(path, &st) != 0)
		return (0);
	return (S_ISDIR(st.st_mode));
}

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
    if (ft_strcmp(cmd_name, ".") == 0 || ft_strcmp(cmd_name, "..") == 0)
        return (NULL);
    path_value = get_env_value(envp, "PATH");
    if (!path_value)
        path_value = "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin";
    dirs = ft_split(path_value, ':');
    if (!dirs)
        return (NULL);
    i = 0;
    while (dirs[i])
    {
        full_path = ft_join_path(dirs[i], cmd_name);
        if (full_path && access(full_path, X_OK) == 0 && !is_directory(full_path))
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
