/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_resolve.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkpg-md- <dkpg-md-@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 00:17:23 by dkpg-md-          #+#    #+#             */
/*   Updated: 2026/05/27 21:20:07 by dkpg-md-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_directory(char *path)
{
	struct stat	st;

	if (stat(path, &st) != 0)
		return (0);
	return (S_ISDIR(st.st_mode));
}

int	has_slash(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '/')
			return (1);
		i++;
	}
	return (0);
}

char	*get_env_value(char **envp, const char *name)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(name);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}

static char	*search_in_dirs(char **dirs, char *cmd_name)
{
	char	*full_path;
	char	*found_noexec;
	int		i;

	i = 0;
	found_noexec = NULL;
	while (dirs[i])
	{
		full_path = ft_join_path(dirs[i], cmd_name);
		if (full_path && access(full_path, X_OK) == 0
			&& !is_directory(full_path))
		{
			free(found_noexec);
			free_split(dirs);
			return (full_path);
		}
		if (full_path && access(full_path, F_OK) == 0
			&& !is_directory(full_path) && !found_noexec)
			found_noexec = ft_strdup(full_path);
		free(full_path);
		i++;
	}
	free_split(dirs);
	return (found_noexec);
}

char	*resolve_command_path(char *cmd_name, char **envp)
{
	char	*path_value;
	char	**dirs;

	if (!cmd_name || !*cmd_name)
		return (NULL);
	if (has_slash(cmd_name))
	{
		if (access(cmd_name, F_OK) == 0)
			return (ft_strdup(cmd_name));
		return (NULL);
	}
	if (ft_strcmp(cmd_name, ".") == 0 || ft_strcmp(cmd_name, "..") == 0)
		return (NULL);
	path_value = get_env_value(envp, "PATH");
	if (!path_value)
		return (NULL);
	dirs = ft_split(path_value, ':');
	if (!dirs)
		return (NULL);
	return (search_in_dirs(dirs, cmd_name));
}
