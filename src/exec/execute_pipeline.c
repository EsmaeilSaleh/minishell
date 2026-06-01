/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkpg-md- <dkpg-md-@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 20:27:14 by dkpg-md-          #+#    #+#             */
/*   Updated: 2026/06/01 15:42:42 by dkpg-md-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_cmds(t_cmd *cmds)
{
	int	count;

	count = 0;
	while (cmds)
	{
		count++;
		cmds = cmds->next;
	}
	return (count);
}

static void	update_last_status(pid_t waited, pid_t last,
		int status, t_shell *shell)
{
	if (waited != last)
		return ;
	if (WIFEXITED(status))
		shell->last_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		shell->last_exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGINT)
			write(1, "\n", 1);
	}
}

static int	fork_and_pipe(t_cmd *current, t_shell *shell, t_pipe_info *info)
{
	pid_t	pid;

	if (current->next && pipe(info->pipefd) == -1)
		return (-1);
	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0)
	{
		info->has_next = current->next != NULL;
		exec_child_process(current, shell, info);
	}
	if (current->next)
		close(info->pipefd[1]);
	if (info->prev_fd != -1)
		close(info->prev_fd);
	if (current->next)
		info->prev_fd = info->pipefd[0];
	else
		info->prev_fd = -1;
	if (current->next == NULL)
		info->last_pid = pid;
	return (0);
}

int	execute_pipeline(t_cmd *cmds, t_shell *shell)
{
	t_pipe_info	info;
	int			status;
	int			cmd_count;
	pid_t		waited_pid;
	t_cmd		*current;

	info.prev_fd = -1;
	info.last_pid = -1;
	current = cmds;
	cmd_count = count_cmds(cmds);
	while (current)
	{
		if (fork_and_pipe(current, shell, &info) == -1)
			return (1);
		current = current->next;
	}
	if (info.prev_fd != -1)
		close(info.prev_fd);
	while (cmd_count-- > 0)
	{
		waited_pid = waitpid(-1, &status, 0);
		update_last_status(waited_pid, info.last_pid, status, shell);
	}
	return (shell->last_exit_status);
}
