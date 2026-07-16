/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_warn.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esaleh <esaleh@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 10:39:54 by esaleh            #+#    #+#             */
/*   Updated: 2026/07/16 10:39:55 by esaleh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_heredoc_eof_warning(char *delimiter)
{
	char	*warn_msg;

	warn_msg = "bash: warning: here-document at line 1 delimited "
		"by end-of-file (wanted `";
	write(2, warn_msg, ft_strlen(warn_msg));
	write(2, delimiter, ft_strlen(delimiter));
	write(2, "')\n", 3);
}
