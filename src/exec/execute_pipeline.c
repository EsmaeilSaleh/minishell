#include "minishell.h"

static int count_cmds(t_cmd *cmds)
{
    int count;

    count = 0;
    while (cmds)
    {
        count++;
        cmds = cmds->next;
    }
    return (count);
}

int execute_pipeline(t_cmd *cmds, t_shell *shell)
{
    int prev_fd;
    int pipefd[2];
    int status;
    int cmd_count;
    pid_t pid;
    pid_t waited_pid;
    pid_t last_pid;
    t_cmd *current;

    prev_fd = -1;
    last_pid = -1;
    current = cmds;
    cmd_count = count_cmds(cmds);
    while (current)
    {
        if (current->next && pipe(pipefd) == -1)
            return (1);
        fflush(stdout);
        pid = fork();
        if (pid < 0)
            return (1);
        if (pid == 0)
            exec_child_process(current, shell, prev_fd, pipefd, current->next != NULL);
        if (current->next)
            close(pipefd[1]);
        if (prev_fd != -1)
            close(prev_fd);
        if (current->next)
            prev_fd = pipefd[0];
        else
            prev_fd = -1;
        if (current->next == NULL)
            last_pid = pid;
        current = current->next;
    }
    if (prev_fd != -1)
        close(prev_fd);
    while (cmd_count > 0)
    {
        waited_pid = waitpid(-1, &status, 0);
        if (waited_pid == last_pid && WIFEXITED(status))
            shell->last_exit_status = WEXITSTATUS(status);
        cmd_count--;
    }
    return (shell->last_exit_status);
}