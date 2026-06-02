/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_single_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkpg-md- <dkpg-md-@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 00:17:23 by dkpg-md-          #+#    #+#             */
/*   Updated: 2026/05/27 20:31:46 by dkpg-md-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_redir_only(t_cmd *cmd, t_shell *shell)
{
	int	stdin_backup;
	int	stdout_backup;

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

int	handle_builtin(t_cmd *cmd, t_shell *shell)
{
	int	stdin_backup;
	int	stdout_backup;

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

void	handle_child(t_cmd *cmd, t_shell *shell, char *path)
{
	set_signal_handler(SIGINT, SIG_DFL);
	set_signal_handler(SIGQUIT, SIG_DFL);
	if (apply_redirs(cmd->redirs) != 0)
		exit(1);
	execve(path, cmd->argv, shell->envp);
	perror("execve");
	exit(126);
}

int	handle_parent(int status, t_shell *shell)
{
	if (WIFEXITED(status))
		shell->last_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		shell->last_exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGINT)
			write(1, "\n", 1);
	}
	return (shell->last_exit_status);
}
