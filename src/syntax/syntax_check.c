#include "minishell.h"

static int	check_pipe_syntax(t_token *tokens)
{
	if (is_pipe_token(tokens))
		return (0);
	while (tokens)
	{
		if (is_pipe_tokens(tokens))
		{
			if (tokens->next == NULL)
				return (0);
			if (is_pipe_token(tokens->next))
				return (0);
			if (is_redir_token(tokens->next))
				return (0);
		}
		tokens = tokens->next;
	}
	return (1);

}

static int	check_redir_syntax(t_token *tokens)
{

}

int	syntax_check(t_token *token)
{

}
