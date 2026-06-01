/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esaleh <esaleh@student.42berlin.de>        +#+  +:+       +#+         */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 00:00:00 by esmaeil           #+#    #+#             */
/*   Updated: 2026/05/27 00:00:00 by esmaeil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**copy_envp(char **envp)
{
	char	**new_env;
	int		count;
	int		i;

	count = env_count(envp);
	new_env = malloc(sizeof(char *) * (count + 1));
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_env[i] = ft_strdup(envp[i]);
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

static void	add_env_entry(t_shell *shell, char *entry)
{
	char	**new_env;
	int		count;
	int		i;

	count = env_count(shell->envp);
	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
	{
		free(entry);
		return ;
	}
	i = 0;
	while (i < count)
	{
		new_env[i] = shell->envp[i];
		i++;
	}
	new_env[count] = entry;
	new_env[count + 1] = NULL;
	free(shell->envp);
	shell->envp = new_env;
}

static void	set_env_entry(t_shell *shell, char *entry, char *key)
{
	int	idx;

	idx = env_find_index(shell->envp, key);
	if (idx != -1)
	{
		free(shell->envp[idx]);
		shell->envp[idx] = entry;
		return ;
	}
	add_env_entry(shell, entry);
}

static void	set_shlvl(t_shell *shell)
{
	char	*val;
	char	*new_val;
	char	*entry;
	int		lvl;

	val = get_env_value(shell->envp, "SHLVL");
	if (val)
		lvl = ft_atoi(val) + 1;
	else
		lvl = 1;
	if (lvl < 0)
		lvl = 0;
	new_val = ft_itoa(lvl);
	if (!new_val)
		return ;
	entry = ft_strjoin("SHLVL=", new_val);
	free(new_val);
	if (entry)
		set_env_entry(shell, entry, "SHLVL");
}

void	init_shell(t_shell *shell, char **envp)
{
	int		idx;
	char	*entry;

	shell->envp = copy_envp(envp);
	shell->last_exit_status = 0;
	shell->running = 1;
	set_shlvl(shell);
	idx = env_find_index(shell->envp, "OLDPWD");
	if (idx == -1 || shell->envp[idx][6] == '=')
	{
		entry = ft_strdup("OLDPWD");
		if (!entry)
			return ;
		set_env_entry(shell, entry, "OLDPWD");
	}
}
