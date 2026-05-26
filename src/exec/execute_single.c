#include "minishell.h"

int execute_single(t_cmd *cmd, t_shell *shell)
{
    char *path;
    pid_t pid;
    int status;
    int stdin_backup;
    int stdout_backup;

    if (cmd == NULL)
        return (0);
    if (cmd->argv == NULL || cmd->argv[0] == NULL)
    {
        stdin_backup = dup(STDIN_FILENO);
        stdout_backup = dup(STDOUT_FILENO);
        if (apply_redirs(cmd->redirs) != 0)
        {
            restore_stdio(stdin_backup, stdout_backup);
            shell->last_exit_status = 1;
            return (1);
        }
        restore_stdio(stdin_backup, stdout_backup);
        shell->last_exit_status = 0;
        return (0);
    }
    if (is_builtin(cmd->argv[0])
        && !(ft_strcmp(cmd->argv[0], "env") == 0 && cmd->argv[1] != NULL))
    {
        stdin_backup = dup(STDIN_FILENO);
        stdout_backup = dup(STDOUT_FILENO);
        if (apply_redirs(cmd->redirs) != 0)
        {
            restore_stdio(stdin_backup, stdout_backup);
            shell->last_exit_status = 1;
            return (1);
        }
        shell->last_exit_status = exec_builtin(cmd, shell);
        restore_stdio(stdin_backup, stdout_backup);
        return (shell->last_exit_status);
    }
    path = resolve_command_path(cmd->argv[0], shell->envp);
    if (path == NULL)
    {
        printf("command not found: %s\n", cmd->argv[0]);
        shell->last_exit_status = 127;
        return (127);
    }
    pid = fork();
    if (pid == 0)
    {
        if (apply_redirs(cmd->redirs) != 0)
            exit(1);
        execve(path, cmd->argv, shell->envp);
        perror("execve");
        exit(126);
    }
    else
    {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            shell->last_exit_status = WEXITSTATUS(status);
    }
    free(path);
    return (shell->last_exit_status);
}
