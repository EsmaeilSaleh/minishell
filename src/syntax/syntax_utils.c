/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esaleh <esaleh@student.42berlin.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 00:00:00 by esaleh            #+#    #+#             */
/*   Updated: 2026/05/28 00:00:00 by esaleh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_pipe_token(t_token *token)
{
	return (token->type == TOK_PIPE);
}

int	is_redir_token(t_token *token)
{
	t_token_type	type;

	type = token->type;
	return (type == TOK_REDIR_IN || type == TOK_REDIR_OUT
		|| type == TOK_APPEND || type == TOK_HEREDOC);
}

int	syntax_error(void)
{
	fprintf(stderr, "syntax error near unexpected token\n");
	return (0);
}
