/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_remove.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esaleh <esaleh@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 10:37:16 by esaleh            #+#    #+#             */
/*   Updated: 2026/07/16 10:37:17 by esaleh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand_internal.h"

static int	copy_quote_text(t_exp *exp, char *word)
{
	char	quote;

	quote = word[exp->i++];
	while (word[exp->i] && word[exp->i] != quote)
	{
		if (!exp_append_char(exp, word[exp->i++], '0'))
			return (0);
	}
	if (word[exp->i] == quote)
		exp->i++;
	return (1);
}

char	*exp_strip_quotes(char *word)
{
	t_exp	exp;

	if (!exp_init(&exp))
		return (NULL);
	while (word[exp.i])
	{
		if ((word[exp.i] == '\'' || word[exp.i] == '"')
			&& !copy_quote_text(&exp, word))
			return (exp_clear(&exp), NULL);
		else if (word[exp.i] != '\'' && word[exp.i] != '"'
			&& !exp_append_char(&exp, word[exp.i++], '0'))
			return (exp_clear(&exp), NULL);
	}
	free(exp.mask);
	return (exp.text);
}

char	*expand_one_word(char *word, t_shell *shell)
{
	char	*mask;
	char	*expanded;

	expanded = expand_masked(word, shell, &mask);
	free(mask);
	return (expanded);
}

void	exp_expand_redirs(t_redir *redirs, t_shell *shell)
{
	char	*target;

	while (redirs)
	{
		if (redirs->type == TOK_HEREDOC)
			target = exp_strip_quotes(redirs->target);
		else
			target = expand_one_word(redirs->target, shell);
		if (target)
		{
			free(redirs->target);
			redirs->target = target;
		}
		redirs = redirs->next;
	}
}

void	expand_cmds(t_cmd *cmds, t_shell *shell)
{
	while (cmds)
	{
		cmds->argv = exp_expand_argv(cmds->argv, shell);
		exp_expand_redirs(cmds->redirs, shell);
		cmds = cmds->next;
	}
}
