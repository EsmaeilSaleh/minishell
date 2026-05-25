#include "minishell.h"

int execute_cmds(t_cmd *cmds, t_shell *shell)
{
    if (cmds == NULL)
        return (0);
    if (prepare_heredoc(cmds) != 0)
    {
        shell->last_exit_status = 1;
        return (1);
    }
    if (cmds->next == NULL)
        return (execute_single(cmds, shell));
    return (execute_pipeline(cmds, shell));
}