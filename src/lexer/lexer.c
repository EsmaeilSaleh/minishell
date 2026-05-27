/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esaleh <esaleh@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 00:00:00 by esaleh            #+#    #+#             */
/*   Updated: 2026/05/27 00:00:00 by esaleh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_digit_str(char *s)
{
	int	i;

	if (!s || !s[0])
		return (0);
	i = 0;
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static int	handle_word(t_token **tokens, char *line, int *i, int *pending_fd)
{
	char		*value;
	t_token		*new_node;

	if (is_space(line[*i]))
		return (*pending_fd = -1, (*i)++, 1);
	value = extract_word(line, i);
	if (value == NULL)
		return (0);
	if (is_digit_str(value) && is_operator_char(line[*i]))
	{
		*pending_fd = ft_atoi(value);
		free(value);
		return (1);
	}
	*pending_fd = -1;
	new_node = new_token(value, TOK_WORD);
	if (new_node == NULL)
		return (free(value), 0);
	add_token_back(tokens, new_node);
	return (1);
}

static char	*read_operator(char *line, int *i, t_token_type *type)
{
	if (line[*i] == '|')
		return (*type = TOK_PIPE, (*i)++, ft_strdup("|"));
	if (line[*i] == '<' && line[*i + 1] == '<')
		return (*type = TOK_HEREDOC, (*i) += 2, ft_strdup("<<"));
	if (line[*i] == '<')
		return (*type = TOK_REDIR_IN, (*i)++, ft_strdup("<"));
	if (line[*i + 1] == '>')
		return (*type = TOK_APPEND, (*i) += 2, ft_strdup(">>"));
	*type = TOK_REDIR_OUT;
	if (line[*i + 1] == '|')
		(*i)++;
	return ((*i)++, ft_strdup(">"));
}

static int	handle_operator(t_token **tokens, char *line, int *i, int fd)
{
	char			*value;
	t_token_type	type;
	t_token			*new_node;

	value = read_operator(line, i, &type);
	if (value == NULL)
		return (0);
	new_node = new_token(value, type);
	if (new_node == NULL)
		return (free(value), 0);
	if (fd != -1 && type != TOK_PIPE)
		new_node->fd = fd;
	add_token_back(tokens, new_node);
	return (1);
}

t_token	*lexer(char *line)
{
	int		i;
	int		pending_fd;
	t_token	*tokens;

	tokens = NULL;
	i = 0;
	pending_fd = -1;
	while (line[i])
	{
		if (is_operator_char(line[i]))
		{
			if (!handle_operator(&tokens, line, &i, pending_fd))
				return (free_tokens(tokens), NULL);
			pending_fd = -1;
		}
		else if (!handle_word(&tokens, line, &i, &pending_fd))
			return (free_tokens(tokens), NULL);
	}
	return (tokens);
}
