#include "minishell.h"

int ft_cd(char **argv, t_shell *shell)
{
    char *path;

    (void)shell;
    if (argv[1] == NULL)
    {
        fprintf(stderr, "cd: missing argument\n");
        return (1);
    }
    path = argv[1];
    if (ft_strcmp(argv[1], "--") == 0)
    {
        path = get_env_value(shell->envp, "HOME");
        if (path == NULL)
        {
            fprintf(stderr, "cd: HOME not set\n");
            return (1);
        }
    }
    if (ft_strcmp(argv[1], "-") == 0)
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
