/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkpg-md- <dkpg-md-@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 00:17:23 by dkpg-md-          #+#    #+#             */
/*   Updated: 2026/05/28 19:52:24 by dkpg-md-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**build_env_array(char **envp, char *new_entry, int count)
{
	char	**new_env;
	int		i;

	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
		return (free(new_entry), NULL);
	i = 0;
	while (i < count)
	{
		new_env[i] = envp[i];
		i++;
	}
	new_env[count] = new_entry;
	new_env[count + 1] = NULL;
	free(envp);
	return (new_env);
}

static char	**env_add_or_replace(char **envp, char *new_entry, char *key)
{
	int	index;

	index = env_find_index(envp, key);
	if (index != -1)
	{
		free(envp[index]);
		envp[index] = new_entry;
		return (envp);
	}
	return (build_env_array(envp, new_entry, env_count(envp)));
}

static int	set_env_entry(char **argv, int i, t_shell *shell, int eq_index)
{
	char	*key;
	char	*new_entry;
	char	**envp_temp;

	key = ft_substr(argv[i], 0, eq_index);
	new_entry = ft_strdup(argv[i]);
	envp_temp = env_add_or_replace(shell->envp, new_entry, key);
	if (envp_temp)
		shell->envp = envp_temp;
	free(key);
	return (0);
}

static int	process_export_arg(char **argv, int i, t_shell *shell)
{
	int	eq_index;

	eq_index = find_equal_sign(argv[i]);
	if (argv[i][0] == '-' && argv[i][1] != '\0')
		return (write(2, "export: invalid option\n", 23), -1);
	if (has_append_syntax(argv[i], eq_index))
		return (0);
	if (!is_valid_identifier(argv[i], eq_index))
	{
		write(2, "export: `", 9);
		write(2, argv[i], ft_strlen(argv[i]));
		write(2, "': not a valid identifier\n", 26);
		return (1);
	}
	if (eq_index == -1)
		return (0);
	return (set_env_entry(argv, i, shell, eq_index));
}

int	ft_export(char **argv, t_shell *shell)
{
	int	i;
	int	status;
	int	ret;

	if (!argv[1])
		return (print_export_no_args(shell));
	i = 1;
	status = 0;
	while (argv[i])
	{
		ret = process_export_arg(argv, i, shell);
		if (ret == -1)
			return (2);
		if (ret == 1)
			status = 1;
		i++;
	}
	return (status);
}
