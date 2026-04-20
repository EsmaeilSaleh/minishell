#include "minishell.h"

int ft_env(char **envp)
{
    int i;
    if (envp == NULL)
        return (0);
    i = 0;
    while (envp[i])
    {
        printf("%s\n", envp[i]);
        i++;
    }
    return (0);
}