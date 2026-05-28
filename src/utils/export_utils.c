/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkpg-md- <dkpg-md-@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 00:17:23 by dkpg-md-          #+#    #+#             */
/*   Updated: 2026/05/28 18:19:41 by dkpg-md-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_equal_sign(char *arg)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (arg[i] == '=')
			return (i);
		i++;
	}
	return (-1);
}

int	is_name_start(char c)
{
	return ((c >= 'A' && c <= 'Z')
		|| (c >= 'a' && c <= 'z') || c == '_');
}

int	is_name_char(char c)
{
	return (is_name_start(c) || (c >= '0' && c <= '9'));
}

int	is_valid_identifier(char *arg, int eq_index)
{
	int	i;

	if (arg == NULL || arg[0] == '\0')
		return (0);
	if (eq_index == -1)
		eq_index = ft_strlen(arg);
	if (eq_index == 0 || !is_name_start(arg[0]))
		return (0);
	i = 1;
	while (i < eq_index)
	{
		if (!is_name_char(arg[i]))
			return (0);
		i++;
	}
	return (1);
}

int	has_append_syntax(char *arg, int eq_index)
{
	return (eq_index > 0 && arg[eq_index - 1] == '+'
		&& is_valid_identifier(arg, eq_index - 1));
}
