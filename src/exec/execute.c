/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkpg-md- <dkpg-md-@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 00:17:23 by dkpg-md-          #+#    #+#             */
/*   Updated: 2026/05/27 18:00:20 by dkpg-md-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_cmds(t_cmd *cmds, t_shell *shell)
{
	int	status;

	if (cmds == NULL)
		return (0);
	status = prepare_heredocs(cmds, shell);
	if (status != 0)
	{
		shell->last_exit_status = status;
		return (status);
	}
	if (cmds->next == NULL)
		return (execute_single(cmds, shell));
	return (execute_pipeline(cmds, shell));
}
