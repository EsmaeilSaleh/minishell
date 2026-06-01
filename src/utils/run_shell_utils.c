/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_shell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkpg-md- <dkpg-md-@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 00:17:23 by dkpg-md-          #+#    #+#             */
/*   Updated: 2026/05/28 19:39:07 by dkpg-md-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	has_unclosed_quotes(char *line)
{
	int	i;
	int	single_quote;
	int	double_quote;

	i = 0;
	single_quote = 0;
	double_quote = 0;
	while (line[i])
	{
		if (line[i] == '\'' && !double_quote)
			single_quote = !single_quote;
		else if (line[i] == '"' && !single_quote)
			double_quote = !double_quote;
		i++;
	}
	return (single_quote || double_quote);
}

int	find_semi_outside_quotes(char *line)
{
	int	i;
	int	sq;
	int	dq;

	i = 0;
	sq = 0;
	dq = 0;
	while (line[i])
	{
		if (line[i] == '\'' && !dq)
			sq = !sq;
		else if (line[i] == '"' && !sq)
			dq = !dq;
		else if (line[i] == ';' && !sq && !dq)
			return (i);
		i++;
	}
	return (-1);
}

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

void	process_tokens(t_shell *shell, char *line, int interactive)
{
	t_token	*tokens;
	t_cmd	*cmds;

	tokens = lexer(line);
	if (!tokens)
		return ;
	cmds = NULL;
	if (syntax_check(tokens))
		cmds = parser(tokens);
	free_tokens(tokens);
	if (!cmds)
	{
		shell->last_exit_status = 2;
		if (!interactive)
			shell->running = 0;
		return ;
	}
	expand_cmds(cmds, shell);
	execute_cmds(cmds, shell);
	free_cmds(cmds);
}

char	*read_non_interactive_line(void)
{
	char	*line;
	char	c;
	int		len;
	int		capacity;
	int		bytes_read;

	line = malloc(128);
	if (!line)
		return (NULL);
	len = 0;
	capacity = 128;
	while (1)
	{
		bytes_read = read(STDIN_FILENO, &c, 1);
		if (bytes_read <= 0)
			break ;
		if (c == '\n')
			break ;
		if (len + 1 >= capacity)
		{
			line = grow_buffer(line, &capacity);
			if (!line)
				return (NULL);
		}
		line[len++] = c;
	}
	if (len == 0)
		return (free(line), NULL);
	line[len] = '\0';
	return (line);
}
