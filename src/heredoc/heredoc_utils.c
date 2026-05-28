/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esaleh <esaleh@student.42berlin.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 00:00:00 by esaleh            #+#    #+#             */
/*   Updated: 2026/05/28 00:00:00 by esaleh           ###   ########.fr       */
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
