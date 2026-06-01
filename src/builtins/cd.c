/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkpg-md- <dkpg-md-@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 00:19:17 by dkpg-md-          #+#    #+#             */
/*   Updated: 2026/06/01 00:00:00 by dkpg-md-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	update_env_var(t_shell *shell, char *key, char *value)
{
	char	*entry;
	char	*tmp;
	char	*argv[3];

	tmp = ft_strjoin(key, "=");
	if (!tmp)
		return ;
	entry = ft_strjoin(tmp, value);
	free(tmp);
	if (!entry)
		return ;
	argv[0] = "export";
	argv[1] = entry;
	argv[2] = NULL;
	ft_export(argv, shell);
	free(entry);
}

static char	*get_cd_path(char **argv, t_shell *shell)
{
	char	*path;

	if (argv[1] == NULL || ft_strcmp(argv[1], "--") == 0)
	{
		path = get_env_value(shell->envp, "HOME");
		if (!path)
			return (write(2, "cd: HOME not set\n", 17), NULL);
		return (path);
	}
	if (argv[2] != NULL)
		return (write(2, "cd: too many arguments\n", 23), NULL);
	if (ft_strcmp(argv[1], "-") == 0)
	{
		path = get_env_value(shell->envp, "OLDPWD");
		if (!path)
			return (write(2, "cd: OLDPWD not set\n", 19), NULL);
		write(1, path, ft_strlen(path));
		write(1, "\n", 1);
		return (path);
	}
	return (argv[1]);
}

int	ft_cd(char **argv, t_shell *shell)
{
	char	*path;
	char	*old_pwd;
	char	*abs_path;
	char	buffer[1024];

	old_pwd = get_env_value(shell->envp, "PWD");
	path = get_cd_path(argv, shell);
	if (path == NULL)
		return (1);
	abs_path = get_abs_path(path, old_pwd);
	if (!abs_path)
		return (1);
	if (chdir(abs_path) != 0)
	{
		perror("cd");
		free(abs_path);
		return (1);
	}
	free(abs_path);
	if (old_pwd)
		update_env_var(shell, "OLDPWD", old_pwd);
	if (getcwd(buffer, sizeof(buffer)))
		update_env_var(shell, "PWD", buffer);
	return (0);
}
