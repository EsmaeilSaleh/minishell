/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child_free.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkpg-md- <dkpg-md-@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 00:00:00 by esaleh            #+#    #+#             */
/*   Updated: 2026/06/02 12:17:44 by dkpg-md-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_child(t_cmd *cmds, t_shell *shell)
{
	free_cmds(cmds);
	free_split(shell->envp);
	rl_clear_history();
}

void	child_exit(t_cmd *cmds_head, t_shell *shell, int status)
{
	free_child(cmds_head, shell);
	exit(status);
}
