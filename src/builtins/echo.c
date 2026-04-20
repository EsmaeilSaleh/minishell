#include "minishell.h"

int ft_echo(char **argv)
{
    int i;
    int newline;

    i = 1;
    newline = 1;
    if (argv[1] && ft_strcmp(argv[1], "-n") == 0)
    {
        newline = 0;
        i = 2;
    }
    while (argv[i])
    {
        printf("%s", argv[i]);
        if (argv[i + 1])
            printf(" ");
        i++;
    }
    if (newline)
        printf("\n");
    return (0);
}