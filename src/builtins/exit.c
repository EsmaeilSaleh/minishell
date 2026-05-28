/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkpg-md- <dkpg-md-@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 00:17:23 by dkpg-md-          #+#    #+#             */
/*   Updated: 2026/05/28 18:08:28 by dkpg-md-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_numeric_arg(char *arg)
{
	int	i;

	if (arg == NULL || arg[0] == '\0')
		return (0);
	i = 0;
	if (arg[i] == '+' || arg[i] == '-')
		i++;
	if (arg[i] == '\0')
		return (0);
	while (arg[i])
	{
		if (arg[i] < '0' || arg[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	ft_exit(char **argv, t_shell *shell)
{
	if (argv[1] == NULL)
	{
		shell->running = 0;
		return (0);
	}
	if (!is_numeric_arg(argv[1]))
	{
		shell->running = 0;
		write(2, "exit: numeric argument required\n", 32);
		return (2);
	}
	if (argv[2] != NULL)
	{
		write(2, "exit: too many arguments\n", 25);
		return (1);
	}
	shell->running = 0;
	return ((unsigned char)ft_atoi(argv[1]));
}
