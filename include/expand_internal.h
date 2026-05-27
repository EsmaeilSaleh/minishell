/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_internal.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esaleh <esaleh@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 00:00:00 by esaleh            #+#    #+#             */
/*   Updated: 2026/05/27 00:00:00 by esaleh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_INTERNAL_H
# define EXPAND_INTERNAL_H

# include "minishell.h"

typedef struct s_exp
{
	char	*text;
	char	*mask;
	int		i;
}	t_exp;

int		exp_init(t_exp *exp);
void	exp_clear(t_exp *exp);
int		exp_append(t_exp *exp, char *text, char mark);
int		exp_append_char(t_exp *exp, char c, char mark);
char	*exp_dollar(char *word, int *i, t_shell *shell);
int		exp_copy_quote(t_exp *exp, char *word, t_shell *shell);
int		exp_has_quotes(char *word);
char	*expand_masked(char *word, t_shell *shell, char **mask);
char	**exp_expand_argv(char **argv, t_shell *shell);
char	*exp_strip_quotes(char *word);
void	exp_expand_redirs(t_redir *redirs, t_shell *shell);

#endif
