/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkpg-md- <dkpg-md-@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 15:08:35 by dkpg-md-          #+#    #+#             */
/*   Updated: 2026/05/27 15:34:54 by dkpg-md-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**copy_without(char **envp, int index, int count)
{
	char	**new_env;
	int		i;
	int		j;

	new_env = malloc(sizeof(char *) * count);
	if (!new_env)
		return (NULL);
	i = 0;
	j = 0;
	while (envp[i])
	{
		if (i != index)
			new_env[j++] = envp[i];
		i++;
	}
	new_env[j] = NULL;
	return (new_env);
}

static char	**env_remove_key(char **envp, char *key)
{
	char	**new_env;
	int		index;
	int		count;

	index = env_find_index(envp, key);
	if (index == -1)
		return (envp);
	count = env_count(envp);
	new_env = copy_without(envp, index, count);
	if (!new_env)
		return (NULL);
	free(envp[index]);
	free(envp);
	return (new_env);
}

int	ft_unset(char **argv, t_shell *shell)
{
	int		i;
	char	**tmp;

	i = 1;
	if (argv[1] == NULL)
		return (0);
	while (argv[i])
	{
		tmp = env_remove_key(shell->envp, argv[i]);
		if (tmp)
			shell->envp = tmp;
		i++;
	}
	return (0);
}
