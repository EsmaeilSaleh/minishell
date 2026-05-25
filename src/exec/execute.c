#include "minishell.h"

void heredoc_sigint_handler(int signum)
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
    rl_done = 0;
    g_signal_status = 0;
    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, SIG_IGN);
}

int execute_cmds(t_cmd *cmds, t_shell *shell)
{
    if (cmds == NULL)
        return (0);
    if (prepare_heredocs(cmds) != 0)
    {
        shell->last_exit_status = 1;
        return (1);
    }
    if (cmds->next == NULL)
        return (execute_single(cmds, shell));
    return (execute_pipeline(cmds, shell));
}