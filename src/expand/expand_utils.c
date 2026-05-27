/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esaleh <esaleh@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 00:00:00 by esaleh            #+#    #+#             */
/*   Updated: 2026/05/27 00:00:00 by esaleh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand_internal.h"

int	exp_init(t_exp *exp)
{
	exp->text = ft_strdup("");
	exp->mask = ft_strdup("");
	exp->i = 0;
	if (!exp->text || !exp->mask)
	{
		exp_clear(exp);
		return (0);
	}
	return (1);
}

void	exp_clear(t_exp *exp)
{
	free(exp->text);
	free(exp->mask);
	exp->text = NULL;
	exp->mask = NULL;
}

int	exp_append(t_exp *exp, char *str, char mark)
{
	char	*text;
	char	*mask;
	int		i;
	int		old;

	text = ft_strjoin(exp->text, str);
	if (!text)
		return (0);
	old = ft_strlen(exp->mask);
	mask = malloc(ft_strlen(text) + 1);
	if (!mask)
		return (free(text), 0);
	i = -1;
	while (++i < old)
		mask[i] = exp->mask[i];
	while (text[i])
		mask[i++] = mark;
	mask[i] = '\0';
	free(exp->text);
	free(exp->mask);
	exp->text = text;
	exp->mask = mask;
	return (1);
}

int	exp_append_char(t_exp *exp, char c, char mark)
{
	char	str[2];

	str[0] = c;
	str[1] = '\0';
	return (exp_append(exp, str, mark));
}
