/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esaleh <esaleh@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 00:00:00 by esaleh            #+#    #+#             */
/*   Updated: 2026/05/27 00:00:00 by esaleh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redir	*new_redir(t_token_type type, char *target)
{
	t_redir	*node;

	node = malloc(sizeof(t_redir));
	if (node == NULL)
	{
		free(target);
		return (NULL);
	}
	node->type = type;
	node->target = target;
	node->heredoc_fd = -1;
	node->expand_body = 0;
	node->next = NULL;
	if (type == TOK_REDIR_IN || type == TOK_HEREDOC)
		node->fd = 0;
	else
		node->fd = 1;
	return (node);
}

void	add_redir_back(t_redir **list, t_redir *new_node)
{
	t_redir	*current;

	if (list == NULL || new_node == NULL)
		return ;
	if (*list == NULL)
	{
		*list = new_node;
		return ;
	}
	current = *list;
	while (current->next)
		current = current->next;
	current->next = new_node;
}
