#include "minishell.h"

void	set_underscore(t_shell *shell, char *path)
{
    char    *entry;
    int     idx;
    int     count;
    char    **new_env;
    int     i;

    entry = ft_strjoin("_=", path);
    if (!entry)
        return ;
    idx = env_find_index(shell->envp, "_");
    if (idx != -1)
    {
        free(shell->envp[idx]);
        shell->envp[idx] = entry;
        return ;
    }
    count = env_count(shell->envp);
    new_env = malloc(sizeof(char *) * (count + 2));
    if (!new_env)
    {
        free(entry);
        return ;
    }
    i = 0;
    while (i < count)
    {
        new_env[i] = shell->envp[i];
        i++;
    }
    new_env[count] = entry;
    new_env[count + 1] = NULL;
    free(shell->envp);
    shell->envp = new_env;
}

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
        fprintf(stderr, "command not found: %s\n", cmd->argv[0]);
        exit(127);
    }
    set_underscore(shell, path);
    execve(path, cmd->argv, shell->envp);
    perror("execve");
    exit(126);
}
