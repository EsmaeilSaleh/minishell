#include "minishell.h"
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