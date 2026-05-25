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
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}
*/
volatile sig_atomic_t g_signal_status = 0;

static void sigint_handler(int signum)
{
	(void)signum;
	g_signal_status = SIGINT;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

static void heredoc_sigint_handler(int signum)
{
	(void)signum;
	g_signal_status = SIGINT;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_done = 1;
}

void setup_signals(void)
{
	g_signal_status = 0;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void setup_heredoc_signals(void)
{
	g_signal_status = 0;
	signal(SIGINT, heredoc_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}