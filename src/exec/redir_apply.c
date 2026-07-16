/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_apply.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkpg-md- <dkpg-md-@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 00:17:23 by dkpg-md-          #+#    #+#             */
/*   Updated: 2026/07/16 10:24:16 by dkpg-md-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	restore_stdio(int stdin_backup, int stdout_backup)
{
	dup2(stdin_backup, STDIN_FILENO);
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdin_backup);
	close(stdout_backup);
}

static void	redir_error(char *target)
{
	write(2, "minishell: ", 11);
	write(2, target, ft_strlen(target));
	write(2, ": ", 2);
	write(2, strerror(errno), ft_strlen(strerror(errno)));
	write(2, "\n", 1);
}

static int	open_redir_file(t_redir *redir)
{
	if (redir->type == TOK_REDIR_IN)
		return (open(redir->target, O_RDONLY));
	if (redir->type == TOK_REDIR_OUT)
		return (open(redir->target, O_WRONLY | O_CREAT | O_TRUNC, 0644));
	if (redir->type == TOK_APPEND)
		return (open(redir->target, O_WRONLY | O_CREAT | O_APPEND, 0644));
	if (redir->type == TOK_HEREDOC)
		return (redir->heredoc_fd);
	return (-1);
}

int	apply_redirs(t_redir *redirs)
{
	int	file_fd;

	while (redirs)
	{
		file_fd = open_redir_file(redirs);
		if (file_fd < 0)
			return (redir_error(redirs->target), 1);
		if (dup2(file_fd, redirs->fd) < 0)
		{
			perror("dup2");
			close(file_fd);
			return (1);
		}
		if (file_fd != redirs->fd)
			close(file_fd);
		if (redirs->type == TOK_HEREDOC)
			redirs->heredoc_fd = -1;
		redirs = redirs->next;
	}
	return (0);
}
