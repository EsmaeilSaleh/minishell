/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkpg-md- <dkpg-md-@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 16:15:04 by dkpg-md-          #+#    #+#             */
/*   Updated: 2026/05/27 20:16:28 by dkpg-md-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_heredoc_signals(void)
{
	rl_done = 0;
	g_signal_status = 0;
	set_signal_handler(SIGINT, heredoc_sigint_handler);
	set_signal_handler(SIGQUIT, SIG_IGN);
}

void	setup_child_signals(void)
{
	set_signal_handler(SIGINT, SIG_DFL);
	set_signal_handler(SIGQUIT, SIG_DFL);
}

void	report_signal_exit(int status)
{
	if (!WIFSIGNALED(status))
		return ;
	if (WTERMSIG(status) == SIGINT)
		write(1, "\n", 1);
	else if (WTERMSIG(status) == SIGQUIT)
	{
		write(2, "Quit", 4);
		if (WCOREDUMP(status))
			write(2, " (core dumped)", 14);
		write(2, "\n", 1);
	}
}
