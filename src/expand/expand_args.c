/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esaleh <esaleh@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 00:00:00 by esaleh            #+#    #+#             */
/*   Updated: 2026/05/27 00:00:00 by esaleh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand_internal.h"

static char	**add_arg(char **args, int *count, char *value)
{
	char	**new_args;
	int		i;

	new_args = malloc(sizeof(char *) * (*count + 2));
	if (!new_args)
		return (NULL);
	i = -1;
	while (++i < *count)
		new_args[i] = args[i];
	new_args[i] = value;
	new_args[i + 1] = NULL;
	free(args);
	(*count)++;
	return (new_args);
}

static char	**split_fields(char *word, char *mask)
{
	char	**fields;
	int		i;
	int		start;
	int		n;

	fields = malloc(sizeof(char *) * (ft_strlen(word) + 1));
	if (!fields)
		return (NULL);
	i = 0;
	n = 0;
	while (word[i])
	{
		while (word[i] && mask[i] == '0' && is_space(word[i]))
			i++;
		start = i;
		while (word[i] && !(mask[i] == '0' && is_space(word[i])))
			i++;
		if (start < i)
			fields[n++] = ft_substr(word, start, i - start);
	}
	fields[n] = NULL;
	return (fields);
}

static char	**add_fields(char **args, int *count, char **fields)
{
	int	i;

	i = 0;
	while (fields && fields[i])
	{
		args = add_arg(args, count, fields[i]);
		if (!args)
			return (NULL);
		i++;
	}
	free(fields);
	return (args);
}

static char	**collect_arg(char **args, int *count, char *raw, t_shell *shell)
{
	char	*word;
	char	*mask;
	char	**fields;

	word = expand_masked(raw, shell, &mask);
	if (!word)
		return (free(mask), args);
	if (!word[0] && exp_has_quotes(raw))
		return (free(mask), add_arg(args, count, word));
	if (!word[0])
		return (free(mask), free(word), args);
	fields = split_fields(word, mask);
	free(mask);
	free(word);
	return (add_fields(args, count, fields));
}

char	**exp_expand_argv(char **argv, t_shell *shell)
{
	char	**args;
	int		count;
	int		i;

	if (!argv)
		return (NULL);
	args = malloc(sizeof(char *));
	if (!args)
		return (argv);
	args[0] = NULL;
	count = 0;
	i = -1;
	while (argv[++i])
	{
		args = collect_arg(args, &count, argv[i], shell);
		free(argv[i]);
	}
	free(argv);
	return (args);
}
