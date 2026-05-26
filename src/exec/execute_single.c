#include "minishell.h"

static int	count_args(char **argv)
{
    int i;

    i = 0;
    while (argv && argv[i])
        i++;
    return (i);
}

static int	has_space(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] == ' ')
            return (1);
        i++;
    }
    return (0);
}

static int	split_command_word(t_cmd *cmd)
{
    char **split;
    char **new_argv;
    int split_count;
    int old_count;
    int i;
    int j;

    split = ft_split(cmd->argv[0], ' ');
    if (split == NULL || split[0] == NULL)
        return (0);
    split_count = count_args(split);
    old_count = count_args(cmd->argv);
    new_argv = malloc(sizeof(char *) * (split_count + old_count));
    if (new_argv == NULL)
    {
        free_split(split);
        return (0);
    }
    i = 0;
    while (i < split_count)
    {
        new_argv[i] = split[i];
        i++;
    }
    j = 1;
    while (j < old_count)
    {
        new_argv[i] = cmd->argv[j];
        i++;
        j++;
    }
    new_argv[i] = NULL;
    free(split);
    free(cmd->argv[0]);
    free(cmd->argv);
    cmd->argv = new_argv;
    return (1);
}

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
    if (cmd->argv[0][0] == '\0')
    {
        shell->last_exit_status = 0;
        return (0);
    }
    if (has_space(cmd->argv[0]))
        split_command_word(cmd);
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
        fprintf(stderr, "command not found: %s\n", cmd->argv[0]);
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
