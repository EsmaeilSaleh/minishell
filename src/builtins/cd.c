#include "minishell.h"

int ft_cd(char **argv, t_shell *shell)
{
    (void)shell;
    if (argv[1] == NULL)
    {
        printf("cd: missing argument\n");
        return (1);
    }
    if (chdir(argv[1]) != 0)
    {
        perror("cd");
        return (1);
    }
    return (0);
}