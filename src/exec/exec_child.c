#include "minishell.h"

void exec_child_process(t_cmd *cmd, t_shell *shell, int prev_fd, int pipefd[2], int has_next)
{
    char *path;
    if (prev_fd != -1)
        dup2(prev_fd, STDIN_FILENO);
    if (has_next)
        dup2(pipefd[1], STDOUT_FILENO);
    if (prev_fd != -1)
        close(prev_fd);
    if (has_next)
    {
        close(pipefd[0]);
        close(pipefd[1]);
    }
    if (apply_redirs(cmd->redirs) != 0)
        exit(1);
    if (cmd->argv == NULL || cmd->argv[0] == NULL)
        exit(0);
    if (is_builtin(cmd->argv[0]))
        exit(exec_builtin(cmd, shell));
    path = resolve_command_path(cmd->argv[0], shell->envp);
    if (path == NULL)
    {
        printf("command not found: %s\n", cmd->argv[0]);
        exit(127);
    }
    execve(path, cmd->argv, shell->envp);
    perror("execve");
    exit(126);
}
