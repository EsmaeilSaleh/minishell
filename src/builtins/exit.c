#include "minishell.h"

int ft_exit(char **argv, t_shell *shell)
{
    if (argv[1] == NULL)
    {
        shell->running = 0;
        return (0);
    }
    shell->running = 0;
    return (ft_atoi(argv[1]));
}