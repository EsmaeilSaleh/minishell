#include "minishell.h"

static char *redir_type_name(t_token_type type)
{
    if (type == TOK_REDIR_IN)
        return ("REDIR_IN");
    if (type == TOK_REDIR_OUT)
        return ("REDIR_OUT");
    if (type == TOK_APPEND)
        return ("APPEND");
    if (type == TOK_HEREDOC)
        return ("HEREDOC");
    return ("UNKNOWN");
}
static void print_argv(char **argv)
{
    int i;

    i = 0;
    if (argv == NULL)
        return;
    while (argv[i])
    {
        printf("    argv[%d] = %s\n", i, argv[i]);
        i++;
    }
}
static void print_redirs(t_redir *redirs)
{
    while (redirs)
    {
        printf("    redir: %s -> %s\n",
               redir_type_name(redirs->type), redirs->target);
        redirs = redirs->next;
    }
}

void print_cmds(t_cmd *cmds)
{
    int cmd_index;

    cmd_index = 1;
    while (cmds)
    {
        printf("CMD %d:\n", cmd_index);
        print_argv(cmds->argv);
        print_redirs(cmds->redirs);
        printf("\n");
        cmds = cmds->next;
        cmd_index++;
    }
}