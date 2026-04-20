#include "minishell.h"

int ft_pwd(void)
{
    char buffer[1024];

    if (getcwd(buffer, sizeof(buffer)) == NULL)
    {
        perror("pwd");
        return (1);
    }
    printf("%s\n", buffer);
    return (0);
}