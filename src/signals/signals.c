#include "minishell.h"

/*
static void sigint_handler(int signum)
{
	(void)signum;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void setup_signals(void)
{
	rl_done = 0;
	g_signal_status = 0;
	set_signal_handler(SIGINT, sigint_handler);
	set_signal_handler(SIGQUIT, SIG_IGN);
}
*/
volatile sig_atomic_t g_signal_status = 0;

void set_signal_handler(int signo, void (*handler)(int))
{
	struct sigaction sa;

	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(signo, &sa, NULL);
}

void sigint_handler(int signum)
{
	(void)signum;
	g_signal_status = SIGINT;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void heredoc_sigint_handler(int signum)
{
	(void)signum;
	g_signal_status = SIGINT;
	write(1, "\n", 1);
	rl_done = 1;
}

void setup_signals(void)
{
	rl_done = 0;
	g_signal_status = 0;
	set_signal_handler(SIGINT, sigint_handler);
	set_signal_handler(SIGQUIT, SIG_IGN);
}

void setup_heredoc_signals(void)
{
	rl_done = 0;
	g_signal_status = 0;
	set_signal_handler(SIGINT, heredoc_sigint_handler);
	set_signal_handler(SIGQUIT, SIG_IGN);
}
