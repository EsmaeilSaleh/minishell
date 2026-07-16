/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esaleh <esaleh@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 10:40:01 by esaleh            #+#    #+#             */
/*   Updated: 2026/07/16 10:40:02 by esaleh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	grow_line(char **line, int *cap, int len)
{
	char	*tmp;
	int		i;

	*cap *= 2;
	tmp = malloc(*cap);
	if (!tmp)
		return (free(*line), 0);
	i = 0;
	while (i < len)
	{
		tmp[i] = (*line)[i];
		i++;
	}
	free(*line);
	*line = tmp;
	return (1);
}

char	*read_line_fd(int fd)
{
	char	*line;
	int		len;
	int		cap;
	char	c;
	int		ret;

	cap = 64;
	len = 0;
	line = malloc(cap);
	if (!line)
		return (NULL);
	while (1)
	{
		ret = read(fd, &c, 1);
		if (ret <= 0 || c == '\n')
			break ;
		if (len + 1 >= cap && !grow_line(&line, &cap, len))
			return (NULL);
		line[len++] = c;
	}
	if (ret <= 0 && len == 0)
		return (free(line), NULL);
	line[len] = '\0';
	return (line);
}

char	*heredoc_next_line(void)
{
	if (isatty(STDIN_FILENO))
		return (readline("> "));
	return (read_line_fd(STDIN_FILENO));
}

void	close_heredoc_fds(t_cmd *cmds)
{
	t_redir	*redir;

	while (cmds)
	{
		redir = cmds->redirs;
		while (redir)
		{
			if (redir->type == TOK_HEREDOC && redir->heredoc_fd >= 0)
			{
				close(redir->heredoc_fd);
				redir->heredoc_fd = -1;
			}
			redir = redir->next;
		}
		cmds = cmds->next;
	}
}

int	process_redirs(t_redir *redir, t_cmd *head, t_shell *shell)
{
	while (redir)
	{
		if (redir->type == TOK_HEREDOC)
		{
			redir->heredoc_fd = prepare_heredoc(redir->target,
					redir->expand_body, shell);
			if (redir->heredoc_fd < 0)
			{
				close_heredoc_fds(head);
				if (g_signal_status == SIGINT)
					return (130);
				return (1);
			}
		}
		redir = redir->next;
	}
	return (0);
}
