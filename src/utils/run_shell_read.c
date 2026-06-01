/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_shell_read.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkpg-md- <dkpg-md-@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 16:09:45 by dkpg-md-          #+#    #+#             */
/*   Updated: 2026/06/01 16:09:48 by dkpg-md-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*grow_buffer(char *line, int *capacity)
{
	char	*new_line;
	int		i;

	new_line = malloc(*capacity * 2);
	if (new_line == NULL)
		return (free(line), NULL);
	i = 0;
	while (i < *capacity)
	{
		new_line[i] = line[i];
		i++;
	}
	free(line);
	*capacity *= 2;
	return (new_line);
}

static char	*read_bytes(char *line, int *len, int *capacity)
{
	char	c;
	int		bytes_read;
	char	*tmp;

	while (1)
	{
		bytes_read = read(STDIN_FILENO, &c, 1);
		if (bytes_read <= 0 || c == '\n')
			break ;
		if (*len + 1 >= *capacity)
		{
			tmp = grow_buffer(line, capacity);
			if (!tmp)
				return (NULL);
			line = tmp;
		}
		line[(*len)++] = c;
	}
	return (line);
}

char	*read_non_interactive_line(void)
{
	char	*line;
	int		len;
	int		capacity;

	line = malloc(128);
	if (!line)
		return (NULL);
	len = 0;
	capacity = 128;
	line = read_bytes(line, &len, &capacity);
	if (!line || len == 0)
		return (free(line), NULL);
	line[len] = '\0';
	return (line);
}
