#include "minishell.h"

char **copy_envp(char **envp)
{
	char **new_env;
	int count;
	int i;

	count = env_count(envp);
	new_env = malloc(sizeof(char *) * (count + 1));
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_env[i] = ft_strdup(envp[i]);
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

static void	set_shlvl(t_shell *shell)
{
	char	*val;
	int		lvl;
	char	*new_val;
	char	*entry;
	int		idx;
	int		count;
	char	**new_env;
	int		i;

	val = get_env_value(shell->envp, "SHLVL");
	if (val)
		lvl = ft_atoi(val) + 1;
	else
		lvl = 1;
	if (lvl < 0)
		lvl = 0;
	new_val = ft_itoa(lvl);
	if (!new_val)
		return ;
	entry = ft_strjoin("SHLVL=", new_val);
	free(new_val);
	if (!entry)
		return ;
	idx = env_find_index(shell->envp, "SHLVL");
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

void init_shell(t_shell *shell, char **envp)
{
	shell->envp = copy_envp(envp);
	shell->last_exit_status = 0;
	shell->running = 1;
	set_shlvl(shell);
}
