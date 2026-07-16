/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_path.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esaleh <esaleh@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 10:43:17 by esaleh            #+#    #+#             */
/*   Updated: 2026/07/16 10:43:18 by esaleh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	read_segment(char **p, char **seg)
{
	int	slen;

	*seg = *p;
	slen = 0;
	while (**p && **p != '/')
	{
		(*p)++;
		slen++;
	}
	if (**p == '/')
		(*p)++;
	return (slen);
}

static void	apply_dotdot(char **w, char *result)
{
	if (*w <= result + 1)
		return ;
	(*w)--;
	while (*w > result + 1 && *(*w - 1) != '/')
		(*w)--;
}

static void	apply_segment(char **w, char *seg, int slen, char *result)
{
	if (slen == 0 || (slen == 1 && seg[0] == '.'))
		return ;
	if (slen == 2 && seg[0] == '.' && seg[1] == '.')
		return (apply_dotdot(w, result));
	while (slen--)
		*(*w)++ = *seg++;
	*(*w)++ = '/';
}

static char	*normalize_abs_path(char *path)
{
	char	*result;
	char	*w;
	char	*p;
	char	*seg;
	int		slen;

	result = malloc(ft_strlen(path) + 2);
	if (!result)
		return (NULL);
	result[0] = '/';
	w = result + 1;
	p = path + (path[0] == '/');
	while (*p)
	{
		slen = read_segment(&p, &seg);
		apply_segment(&w, seg, slen, result);
	}
	if (w > result + 1)
		w -= (*(w - 1) == '/');
	*w = '\0';
	return (result);
}

char	*get_abs_path(char *path, char *pwd)
{
	char	*tmp;
	char	*joined;
	char	*result;

	if (path[0] == '/')
		return (normalize_abs_path(path));
	if (!pwd)
		return (ft_strdup(path));
	tmp = ft_strjoin(pwd, "/");
	if (!tmp)
		return (NULL);
	joined = ft_strjoin(tmp, path);
	free(tmp);
	if (!joined)
		return (NULL);
	result = normalize_abs_path(joined);
	free(joined);
	return (result);
}
