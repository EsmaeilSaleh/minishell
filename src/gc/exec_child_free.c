/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child_free.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esaleh <esaleh@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 10:39:21 by esaleh            #+#    #+#             */
/*   Updated: 2026/07/16 10:39:23 by esaleh           ###   ########.fr       */
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
