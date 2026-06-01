/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_split_str.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkpg-md- <dkpg-md-@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 00:00:00 by esmaeil           #+#    #+#             */
/*   Updated: 2026/06/01 15:44:37 by dkpg-md-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && (s1[i] || s2[i]))
	{
		if ((unsigned char)s1[i] != (unsigned char)s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

char	*ft_strjoin(const char *s1, const char *s2)
{
	char	*joined;
	char	*p;

	joined = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!joined)
		return (NULL);
	p = joined;
	while (*s1)
		*p++ = *s1++;
	while (*s2)
		*p++ = *s2++;
	*p = '\0';
	return (joined);
}

char	*ft_join_path(char *dir, char *cmd)
{
	char	*path;
	size_t	i;

	i = 0;
	path = malloc(ft_strlen(dir) + ft_strlen(cmd) + 2);
	if (!path)
		return (NULL);
	while (*dir)
		path[i++] = *dir++;
	path[i++] = '/';
	while (*cmd)
		path[i++] = *cmd++;
	path[i] = '\0';
	return (path);
}

static size_t	split_pass(const char *s, char c, char **res)
{
	size_t	i;
	size_t	start;
	size_t	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		start = i;
		while (s[i] && s[i] != c)
			i++;
		if (start < i && res)
		{
			res[count] = ft_substr(s, start, i - start);
			if (!res[count])
				return (free_split(res), (size_t) - 1);
			res[++count] = NULL;
		}
		else if (start < i)
			count++;
	}
	return (count);
}

char	**ft_split(const char *s, char c)
{
	char	**res;
	size_t	words;

	if (!s)
		return (NULL);
	words = split_pass(s, c, NULL);
	res = malloc(sizeof(char *) * (words + 1));
	if (!res)
		return (NULL);
	res[0] = NULL;
	if (split_pass(s, c, res) == (size_t) - 1)
		return (NULL);
	return (res);
}
