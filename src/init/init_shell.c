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
void init_shell(t_shell *shell, char **envp)
{
	shell->envp = copy_envp(envp);
	shell->last_exit_status = 0;
	shell->running = 1;
}
