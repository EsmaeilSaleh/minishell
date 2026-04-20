#include "minishell.h"

int execute_single(t_cmd *cmd, t_shell *shell)
{
    char *path;
    pid_t pid;
    int status;

    if (cmd == NULL || cmd->argv == NULL || cmd->argv[0] == NULL)
        return (0);
    if (is_builtin(cmd->argv[0]))
    {
        shell->last_exit_status = exec_builtin(cmd, shell);
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