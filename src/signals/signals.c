/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkpg-md- <dkpg-md-@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 16:07:40 by dkpg-md-          #+#    #+#             */
/*   Updated: 2026/05/27 16:16:45 by dkpg-md-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal_status = 0;

void	set_signal_handler(int signo, void (*handler)(int))
{
	struct sigaction	sa;

	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(signo, &sa, NULL);
}

void	sigint_handler(int signum)
{
	(void)signum;
	g_signal_status = SIGINT;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
}

static int	check_sigint(void)
{
	if (g_signal_status == SIGINT)
	{
		rl_done = 1;
		return (1);
	}
	return (0);
}

void	heredoc_sigint_handler(int signum)
{
	(void)signum;
	g_signal_status = SIGINT;
	write(1, "\n", 1);
	close(STDIN_FILENO);
}

void	setup_signals(void)
{
	g_signal_status = 0;
	rl_event_hook = check_sigint;
	set_signal_handler(SIGINT, sigint_handler);
	set_signal_handler(SIGQUIT, SIG_IGN);
}
