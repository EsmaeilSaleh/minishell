#include "minishell.h"

int is_builtin(char *cmd_name)
{
    if (cmd_name == NULL)
        return (0);
    if (ft_strcmp(cmd_name, "echo") == 0)
        return (1);
    if (ft_strcmp(cmd_name, "cd") == 0)
        return (1);
    if (ft_strcmp(cmd_name, "pwd") == 0)
        return (1);
    if (ft_strcmp(cmd_name, "export") == 0)
        return (1);
    if (ft_strcmp(cmd_name, "unset") == 0)
        return (1);
    if (ft_strcmp(cmd_name, "env") == 0)
        return (1);
    if (ft_strcmp(cmd_name, "exit") == 0)
        return (1);
    return (0);
}

int exec_builtin(t_cmd *cmd, t_shell *shell)
{
    if (ft_strcmp(cmd->argv[0], "echo") == 0)
        return (ft_echo(cmd->argv));
    if (ft_strcmp(cmd->argv[0], "cd") == 0)
        return (ft_cd(cmd->argv, shell));
    if (ft_strcmp(cmd->argv[0], "pwd") == 0)
        return (ft_pwd());
    if (ft_strcmp(cmd->argv[0], "env") == 0)
        return (ft_env(shell->envp));
    if (ft_strcmp(cmd->argv[0], "exit") == 0)
        return (ft_exit(cmd->argv, shell));
    if (ft_strcmp(cmd->argv[0], "export") == 0)
        return (ft_export(cmd->argv, shell));
    if (ft_strcmp(cmd->argv[0], "unset") == 0)
        return (0);
    return (1);
}