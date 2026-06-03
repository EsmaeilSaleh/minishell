/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_single.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkpg-md- <dkpg-md-@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 00:17:23 by dkpg-md-          #+#    #+#             */
/*   Updated: 2026/06/03 13:51:20 by dkpg-md-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	fork_and_exec(t_cmd *cmd, t_shell *shell, char *path)
{
	pid_t	pid;
	int		status;

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
