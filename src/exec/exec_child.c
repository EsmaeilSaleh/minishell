/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkpg-md- <dkpg-md-@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 00:17:23 by dkpg-md-          #+#    #+#             */
/*   Updated: 2026/05/27 21:21:58 by dkpg-md-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**build_new_env(char **envp, char *entry, int count)
{
	char	**new_env;
	int		i;

	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
		return (free(entry), NULL);
	i = 0;
	while (i < count)
	{
		new_env[i] = envp[i];
		i++;
	}
	new_env[count] = entry;
	new_env[count + 1] = NULL;
	return (new_env);
}

void	set_underscore(t_shell *shell, char *path)
{
	char	*entry;
	int		idx;
	char	**new_env;

	entry = ft_strjoin("_=", path);
	if (!entry)
		return ;
	idx = env_find_index(shell->envp, "_");
	if (idx != -1)
	{
		free(shell->envp[idx]);
		shell->envp[idx] = entry;
		return ;
	}
	new_env = build_new_env(shell->envp, entry, env_count(shell->envp));
	if (!new_env)
		return ;
	free(shell->envp);
	shell->envp = new_env;
}

static void	setup_child_fds(int prev_fd, int pipefd[2], int has_next)
{
	if (prev_fd != -1)
		dup2(prev_fd, STDIN_FILENO);
	if (has_next)
		dup2(pipefd[1], STDOUT_FILENO);
	if (prev_fd != -1)
		close(prev_fd);
	if (has_next)
	{
		close(pipefd[0]);
		close(pipefd[1]);
	}
}

static void	exec_cmd(t_cmd *cmd, t_shell *shell)
{
	char	*path;

	if (cmd->argv == NULL || cmd->argv[0] == NULL)
		exit(0);
	if (is_builtin(cmd->argv[0])
		&& !(ft_strcmp(cmd->argv[0], "env") == 0 && cmd->argv[1] != NULL))
		exit(exec_builtin(cmd, shell));
	path = resolve_command_path(cmd->argv[0], shell->envp);
	if (path == NULL)
	{
		write(2, "minishell: command not found: ", 30);
		write(2, cmd->argv[0], ft_strlen(cmd->argv[0]));
		write(2, "\n", 1);
		exit(127);
	}
	set_underscore(shell, path);
	execve(path, cmd->argv, shell->envp);
	perror("execve");
	exit(126);
}

void	exec_child_process(t_cmd *cmd, t_shell *shell, t_pipe_info *info)
{
	setup_child_fds(info->prev_fd, info->pipefd, info->has_next);
	if (apply_redirs(cmd->redirs) != 0)
		exit(1);
	exec_cmd(cmd, shell);
}

/*
void	set_underscore(t_shell *shell, char *path)
{
    char    *entry;
    int     idx;
    int     count;
    char    **new_env;
    int     i;

    entry = ft_strjoin("_=", path);
    if (!entry)
        return ;
    idx = env_find_index(shell->envp, "_");
    if (idx != -1)
    {
        free(shell->envp[idx]);
        shell->envp[idx] = entry;
        return ;
    }
    count = env_count(shell->envp);
    new_env = malloc(sizeof(char *) * (count + 2));
    if (!new_env)
    {
        free(entry);
        return ;
    }
    i = 0;
    while (i < count)
    {
        new_env[i] = shell->envp[i];
        i++;
    }
    new_env[count] = entry;
    new_env[count + 1] = NULL;
    free(shell->envp);
    shell->envp = new_env;
}

void exec_child_process(t_cmd *cmd, t_shell *shell, int prev_fd, int pipefd[2], int has_next)
{
    char *path;
    if (prev_fd != -1)
        dup2(prev_fd, STDIN_FILENO);
    if (has_next)
        dup2(pipefd[1], STDOUT_FILENO);
    if (prev_fd != -1)
        close(prev_fd);
    if (has_next)
    {
        close(pipefd[0]);
        close(pipefd[1]);
    }
    if (apply_redirs(cmd->redirs) != 0)
        exit(1);
    if (cmd->argv == NULL || cmd->argv[0] == NULL)
        exit(0);
    if (is_builtin(cmd->argv[0])
        && !(ft_strcmp(cmd->argv[0], "env") == 0 && cmd->argv[1] != NULL))
        exit(exec_builtin(cmd, shell));
    path = resolve_command_path(cmd->argv[0], shell->envp);
    if (path == NULL)
    {
        fprintf(stderr, "command not found: %s\n", cmd->argv[0]);
        exit(127);
    }
    set_underscore(shell, path);
    execve(path, cmd->argv, shell->envp);
    perror("execve");
    exit(126);
}
*/