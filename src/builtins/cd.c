#include "minishell.h"

static void	set_env_var(t_shell *shell, const char *key, const char *value)
{
	char	*key_eq;
	char	*entry;
	int		idx;
	int		count;
	char	**new_env;
	int		i;

	key_eq = ft_strjoin(key, "=");
	if (!key_eq)
		return ;
	entry = ft_strjoin(key_eq, value);
	free(key_eq);
	if (!entry)
		return ;
	idx = env_find_index(shell->envp, (char *)key);
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

int	ft_cd(char **argv, t_shell *shell)
{
	char	*path;
	char	cwd[4096];
	int		print_new_path;
	char	*old_pwd;

	print_new_path = 0;
	if (argv[1] != NULL && argv[2] != NULL)
	{
		fprintf(stderr, "cd: too many arguments\n");
		return (1);
	}
	if (argv[1] == NULL || ft_strcmp(argv[1], "--") == 0)
	{
		path = get_env_value(shell->envp, "HOME");
		if (path == NULL)
		{
			fprintf(stderr, "cd: HOME not set\n");
			return (1);
		}
	}
	else if (ft_strcmp(argv[1], "-") == 0)
	{
		path = get_env_value(shell->envp, "OLDPWD");
		if (path == NULL)
		{
			fprintf(stderr, "cd: OLDPWD not set\n");
			return (1);
		}
		print_new_path = 1;
	}
	else
		path = argv[1];
	old_pwd = get_env_value(shell->envp, "PWD");
	if (chdir(path) != 0)
	{
		perror("cd");
		return (1);
	}
	if (old_pwd)
		set_env_var(shell, "OLDPWD", old_pwd);
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		set_env_var(shell, "PWD", cwd);
		if (print_new_path)
			printf("%s\n", cwd);
	}
	else if (print_new_path)
		printf("%s\n", path);
	return (0);
}
