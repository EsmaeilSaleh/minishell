/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_display.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkpg-md- <dkpg-md-@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 00:17:23 by dkpg-md-          #+#    #+#             */
/*   Updated: 2026/05/28 18:20:45 by dkpg-md-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	export_name_cmp(char *a, char *b)
{
	int	i;

	i = 0;
	while (a[i] && b[i])
	{
		if (a[i] == '=' || b[i] == '=')
			break ;
		if (a[i] != b[i])
			return ((unsigned char)a[i] - (unsigned char)b[i]);
		i++;
	}
	if ((a[i] == '\0' || a[i] == '=') && (b[i] == '\0' || b[i] == '='))
		return (0);
	if (a[i] == '\0' || a[i] == '=')
		return (-1);
	return (1);
}

static void	sort_env_copy(char **arr, int count)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (export_name_cmp(arr[j], arr[j + 1]) > 0)
			{
				tmp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

static void	print_declare_entry(char *entry)
{
	int	eq;
	int	i;

	eq = find_equal_sign(entry);
	write(1, "declare -x ", 11);
	if (eq == -1)
		return (write(1, entry, ft_strlen(entry)), write(1, "\n", 1), (void)0);
	write(1, entry, eq);
	write(1, "=\"", 2);
	i = eq + 1;
	while (entry[i])
	{
		if (entry[i] == '\\' || entry[i] == '"'
			|| entry[i] == '$' || entry[i] == '`')
			write(1, "\\", 1);
		write(1, &entry[i], 1);
		i++;
	}
	write(1, "\"\n", 2);
}

int	print_export_no_args(t_shell *shell)
{
	int		count;
	char	**sorted;
	int		i;

	count = env_count(shell->envp);
	sorted = malloc(sizeof(char *) * (count + 1));
	if (!sorted)
		return (1);
	i = 0;
	while (i < count)
	{
		sorted[i] = shell->envp[i];
		i++;
	}
	sorted[i] = NULL;
	sort_env_copy(sorted, count);
	i = 0;
	while (sorted[i])
	{
		print_declare_entry(sorted[i]);
		i++;
	}
	free(sorted);
	return (0);
}
