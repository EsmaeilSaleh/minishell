/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkpg-md- <dkpg-md-@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 00:17:23 by dkpg-md-          #+#    #+#             */
/*   Updated: 2026/05/27 15:07:05 by dkpg-md-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd_name)
{
	const char	*builtins[] = {
		"echo", "cd", "pwd", "export",
		"unset", "env", "exit", NULL
	};
	int			i;

	if (!cmd_name)
		return (0);
	i = 0;
	while (builtins[i])
	{
		if (ft_strcmp(cmd_name, builtins[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	exec_builtin(t_cmd *cmd, t_shell *shell)
{
	if (ft_strcmp(cmd->argv[0], "echo") == 0)
		return (ft_echo(cmd->argv));
	if (ft_strcmp(cmd->argv[0], "cd") == 0)
		return (ft_cd(cmd->argv, shell));
	if (ft_strcmp(cmd->argv[0], "pwd") == 0)
		return (ft_pwd(shell));
	if (ft_strcmp(cmd->argv[0], "env") == 0)
		return (ft_env(shell->envp));
	if (ft_strcmp(cmd->argv[0], "exit") == 0)
		return (ft_exit(cmd->argv, shell));
	if (ft_strcmp(cmd->argv[0], "export") == 0)
		return (ft_export(cmd->argv, shell));
	if (ft_strcmp(cmd->argv[0], "unset") == 0)
		return (ft_unset(cmd->argv, shell));
	return (1);
}
