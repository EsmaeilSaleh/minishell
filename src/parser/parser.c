/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esaleh <esaleh@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 10:38:12 by esaleh            #+#    #+#             */
/*   Updated: 2026/07/16 10:38:14 by esaleh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*parser(t_token *tokens)
{
	t_cmd	*cmds;
	t_cmd	*new_node;

	cmds = NULL;
	while (tokens)
	{
		new_node = parse_command(tokens);
		if (new_node == NULL)
		{
			free_cmds(cmds);
			return (NULL);
		}
		add_cmd_back(&cmds, new_node);
		while (tokens && tokens->type != TOK_PIPE)
			tokens = tokens->next;
		if (tokens && tokens->type == TOK_PIPE)
			tokens = tokens->next;
	}
	return (cmds);
}
