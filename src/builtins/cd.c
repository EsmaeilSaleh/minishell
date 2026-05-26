#include "minishell.h"

int ft_cd(char **argv, t_shell *shell)
{
    char *path;
    char cwd[4096];
    int print_new_path;

    print_new_path = 0;
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
        print_new_path = 1;
    }
    if (chdir(path) != 0)
    {
        perror("cd");
        return (1);
    }
    if (print_new_path)
    {
        if (getcwd(cwd, sizeof(cwd)) != NULL)
            printf("%s\n", cwd);
        else
            printf("%s\n", path);
    }
    return (0);
}
