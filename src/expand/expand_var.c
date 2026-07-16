/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esaleh <esaleh@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 10:37:09 by esaleh            #+#    #+#             */
/*   Updated: 2026/07/16 10:37:10 by esaleh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand_internal.h"

static int	is_var_char(char c)
{
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')
		|| (c >= '0' && c <= '9') || c == '_');
}

char	*exp_dollar(char *word, int *i, t_shell *shell)
{
	int		end;
	char	*name;
	char	*value;

	if (word[*i + 1] == '?')
	{
		*i += 2;
		return (ft_itoa(shell->last_exit_status));
	}
	end = *i + 1;
	while (word[end] && is_var_char(word[end]))
		end++;
	if (end == *i + 1)
		return ((*i)++, ft_strdup("$"));
	name = ft_substr(word, *i + 1, end - (*i + 1));
	if (!name)
		return (NULL);
	value = get_env_value(shell->envp, name);
	free(name);
	*i = end;
	if (!value)
		return (ft_strdup(""));
	return (ft_strdup(value));
}

int	exp_copy_quote(t_exp *exp, char *word, t_shell *shell)
{
	char	quote;
	char	*piece;

	quote = word[exp->i++];
	while (word[exp->i] && word[exp->i] != quote)
	{
		if (quote == '"' && word[exp->i] == '$')
		{
			piece = exp_dollar(word, &exp->i, shell);
			if (!piece || !exp_append(exp, piece, '1'))
				return (free(piece), 0);
			free(piece);
		}
		else if (!exp_append_char(exp, word[exp->i++], '1'))
			return (0);
	}
	if (word[exp->i] == quote)
		exp->i++;
	return (1);
}

int	exp_has_quotes(char *word)
{
	while (*word)
	{
		if (*word == '\'' || *word == '"')
			return (1);
		word++;
	}
	return (0);
}
