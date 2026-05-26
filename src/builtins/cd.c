#include "minishell.h"

int ft_cd(char **argv, t_shell *shell)
{
    char *path;

    if (argv[1] == NULL)
    {
        path = get_env_value(shell->envp, "HOME");
        if (path == NULL)
        {
            fprintf(stderr, "cd: HOME not set\n");
            return (1);
        }
    }
    else if (argv[2] != NULL)
    {
        fprintf(stderr, "cd: too many arguments\n");
        return (1);
    }
    else
        path = argv[1];
    if (argv[1] != NULL && ft_strcmp(argv[1], "--") == 0)
    {
        path = get_env_value(shell->envp, "HOME");
        if (path == NULL)
        {
            fprintf(stderr, "cd: HOME not set\n");
            return (1);
        }
    }
    if (argv[1] != NULL && ft_strcmp(argv[1], "-") == 0)
    {
        path = get_env_value(shell->envp, "OLDPWD");
        if (path == NULL)
        {
            fprintf(stderr, "cd: OLDPWD not set\n");
            return (1);
        }
        printf("%s\n", path);
    }
    if (chdir(path) != 0)
    {
        perror("cd");
        return (1);
    }
    return (0);
}
