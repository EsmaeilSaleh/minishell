/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esaleh <esaleh@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 00:00:00 by esaleh            #+#    #+#             */
/*   Updated: 2026/05/27 00:00:00 by esaleh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	token_has_quotes(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '"')
			return (1);
		i++;
	}
	return (0);
}

int	count_cmd_args(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens && tokens->type != TOK_PIPE)
	{
		if (is_redir_token(tokens))
			tokens = tokens->next->next;
		else if (tokens->type == TOK_WORD)
		{
			count++;
			tokens = tokens->next;
		}
		else
			tokens = tokens->next;
	}
	return (count);
}

static int	add_cmd_redir(t_redir **redirs, t_token *token)
{
	char	*target;
	t_redir	*node;

	target = ft_strdup(token->next->value);
	if (!target)
		return (0);
	node = new_redir(token->type, target);
	if (!node)
		return (0);
	if (token->fd != -1)
		node->fd = token->fd;
	if (token->type == TOK_HEREDOC)
		node->expand_body = !token_has_quotes(token->next->value);
	add_redir_back(redirs, node);
	return (1);
}

static int	add_cmd_word(char **argv, int *i, t_token *token)
{
	argv[*i] = ft_strdup(token->value);
	if (!argv[*i])
		return (0);
	(*i)++;
	argv[*i] = NULL;
	return (1);
}

t_cmd	*parse_command(t_token *tokens)
{
	char	**argv;
	t_redir	*redirs;
	int		i;

	argv = malloc(sizeof(char *) * (count_cmd_args(tokens) + 1));
	if (!argv)
		return (NULL);
	argv[0] = NULL;
	redirs = NULL;
	i = 0;
	while (tokens && tokens->type != TOK_PIPE)
	{
		if (is_redir_token(tokens) && !add_cmd_redir(&redirs, tokens))
			return (free_split(argv), free_redirs(redirs), NULL);
		if (tokens->type == TOK_WORD && !add_cmd_word(argv, &i, tokens))
			return (free_split(argv), free_redirs(redirs), NULL);
		if (is_redir_token(tokens))
			tokens = tokens->next;
		tokens = tokens->next;
	}
	return (new_cmd(argv, redirs));
}
