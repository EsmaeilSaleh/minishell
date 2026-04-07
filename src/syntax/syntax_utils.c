#include "minishell.h"

int is_pipe_token(t_token *token)
{
	return (token->type == TOK_PIPE);
}

int	is_redir_token(t_token *token)
{
	t_token_type type = token->type;
	return (type == TOK_REDIR_IN || type == TOK_REDIR_OUT || type == TOK_APPEND || type == TOK_HEREDOC);
}

int	syntax_error(void)
{
	printf("syntax error near unexpected token\n");
	return (0);
}
