/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esaleh <esaleh@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 00:00:00 by esaleh            #+#    #+#             */
/*   Updated: 2026/05/27 00:00:00 by esaleh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand_internal.h"

static int	expand_next(t_exp *exp, char *word, t_shell *shell)
{
	char	*piece;

	if (word[exp->i] == '\\' && word[exp->i + 1])
		return (exp_append_char(exp, word[(exp->i += 2) - 1], '0'));
	if (word[exp->i] == '\\')
		return (exp_append_char(exp, word[exp->i++], '0'));
	if (word[exp->i] == '$' && (word[exp->i + 1] == '\''
			|| word[exp->i + 1] == '"'))
	{
		exp->i++;
		return (exp_copy_quote(exp, word, shell));
	}
	if (word[exp->i] == '\'' || word[exp->i] == '"')
		return (exp_copy_quote(exp, word, shell));
	if (word[exp->i] != '$')
		return (exp_append_char(exp, word[exp->i++], '0'));
	piece = exp_dollar(word, &exp->i, shell);
	if (!piece || !exp_append(exp, piece, '0'))
		return (free(piece), 0);
	free(piece);
	return (1);
}

char	*expand_masked(char *word, t_shell *shell, char **mask)
{
	t_exp	exp;
	char	*home;

	*mask = NULL;
	if (!word || !exp_init(&exp))
		return (NULL);
	home = NULL;
	if (word[0] == '~' && (word[1] == '\0' || word[1] == '/'))
		home = get_env_value(shell->envp, "HOME");
	if (home && !exp_append(&exp, home, '0'))
		return (exp_clear(&exp), NULL);
	if (home)
		exp.i = 1;
	while (word[exp.i] && expand_next(&exp, word, shell))
		;
	if (word[exp.i])
		return (exp_clear(&exp), NULL);
	*mask = exp.mask;
	return (exp.text);
}

static int	heredoc_next(t_exp *exp, char *line, t_shell *shell)
{
	char	*piece;

	if (line[exp->i] == '$')
	{
		piece = exp_dollar(line, &exp->i, shell);
		if (!piece || !exp_append(exp, piece, '0'))
			return (free(piece), 0);
		free(piece);
		return (1);
	}
	if (line[exp->i] == '\\' && (line[exp->i + 1] == '\\'
			|| line[exp->i + 1] == '$' || line[exp->i + 1] == '`'))
		exp->i++;
	if (!exp_append_char(exp, line[exp->i], '0'))
		return (0);
	exp->i++;
	return (1);
}

char	*expand_heredoc_body_line(char *line, t_shell *shell)
{
	t_exp	exp;

	if (!line || !exp_init(&exp))
		return (NULL);
	while (line[exp.i] && heredoc_next(&exp, line, shell))
		;
	free(exp.mask);
	if (line[exp.i])
		return (free(exp.text), NULL);
	return (exp.text);
}
