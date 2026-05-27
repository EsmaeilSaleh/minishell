/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_single.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkpg-md- <dkpg-md-@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 00:17:23 by dkpg-md-          #+#    #+#             */
/*   Updated: 2026/05/27 21:44:36 by dkpg-md-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	fork_and_exec(t_cmd *cmd, t_shell *shell, char *path)
{
	pid_t	pid;
	int		status;

	fflush(stdout);
	set_signal_handler(SIGINT, SIG_IGN);
	pid = fork();
	if (pid < 0)
		return (free(path), 1);
	if (pid == 0)
	{
		set_signal_handler(SIGINT, SIG_DFL);
		handle_child(cmd, shell, path);
	}
	waitpid(pid, &status, 0);
	setup_signals();
	free(path);
	return (handle_parent(status, shell));
}

int	execute_single(t_cmd *cmd, t_shell *shell)
{
	char	*path;

	if (cmd == NULL)
		return (0);
	if (cmd->argv == NULL || cmd->argv[0] == NULL)
		return (handle_redir_only(cmd, shell));
	if (is_builtin(cmd->argv[0])
		&& !(ft_strcmp(cmd->argv[0], "env") == 0 && cmd->argv[1] != NULL))
		return (handle_builtin(cmd, shell));
	path = resolve_command_path(cmd->argv[0], shell->envp);
	if (path == NULL)
	{
		write(2, "minishell: command not found: ", 30);
		write(2, cmd->argv[0], ft_strlen(cmd->argv[0]));
		write(2, "\n", 1);
		shell->last_exit_status = 127;
		return (127);
	}
	return (fork_and_exec(cmd, shell, path));
}

/*
old code
int execute_single(t_cmd *cmd, t_shell *shell)
{
    char *path;
    pid_t pid;
    int status;
    int stdin_backup;
    int stdout_backup;
    int stderr_backup;

    if (cmd == NULL)
        return (0);
    if (cmd->argv == NULL || cmd->argv[0] == NULL)
    {
        stdin_backup = dup(STDIN_FILENO);
        stdout_backup = dup(STDOUT_FILENO);
        stderr_backup = dup(STDERR_FILENO);
        if (apply_redirs(cmd->redirs) != 0)
        {
            restore_stdio(stdin_backup, stdout_backup);
            dup2(stderr_backup, STDERR_FILENO);
            close(stderr_backup);
            shell->last_exit_status = 1;
            return (1);
        }
        restore_stdio(stdin_backup, stdout_backup);
        dup2(stderr_backup, STDERR_FILENO);
        close(stderr_backup);
        shell->last_exit_status = 0;
        return (0);
    }
    if (is_builtin(cmd->argv[0])
        && !(ft_strcmp(cmd->argv[0], "env") == 0 && cmd->argv[1] != NULL))
    {
        stdin_backup = dup(STDIN_FILENO);
        stdout_backup = dup(STDOUT_FILENO);
        stderr_backup = dup(STDERR_FILENO);
        if (apply_redirs(cmd->redirs) != 0)
        {
            restore_stdio(stdin_backup, stdout_backup);
            dup2(stderr_backup, STDERR_FILENO);
            close(stderr_backup);
            shell->last_exit_status = 1;
            return (1);
        }
        shell->last_exit_status = exec_builtin(cmd, shell);
        fflush(stdout);
        restore_stdio(stdin_backup, stdout_backup);
        dup2(stderr_backup, STDERR_FILENO);
        close(stderr_backup);
        return (shell->last_exit_status);
    }
    path = resolve_command_path(cmd->argv[0], shell->envp);
    if (path == NULL)
    {
        fprintf(stderr, "command not found: %s\n", cmd->argv[0]);
        shell->last_exit_status = 127;
        return (127);
    }
    fflush(stdout);
    pid = fork();
    if (pid == 0)
    {
        if (apply_redirs(cmd->redirs) != 0)
            exit(1);
        set_underscore(shell, path);
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
*/
