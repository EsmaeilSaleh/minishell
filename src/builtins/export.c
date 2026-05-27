#include "minishell.h"

static int find_equal_sign(char *arg)
{
    int i;

    i = 0;
    while (arg[i])
    {
        if (arg[i] == '=')
            return (i);
        i++;
    }
    return (-1);
}

static int is_name_start(char c)
{
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_');
}

static int is_name_char(char c)
{
    return (is_name_start(c) || (c >= '0' && c <= '9'));
}

static int is_valid_identifier(char *arg, int eq_index)
{
    int i;

    if (arg == NULL || arg[0] == '\0')
        return (0);
    if (eq_index == -1)
        eq_index = ft_strlen(arg);
    if (eq_index == 0 || !is_name_start(arg[0]))
        return (0);
    i = 1;
    while (i < eq_index)
    {
        if (!is_name_char(arg[i]))
            return (0);
        i++;
    }
    return (1);
}

static int has_append_syntax(char *arg, int eq_index)
{
    return (eq_index > 0 && arg[eq_index - 1] == '+'
        && is_valid_identifier(arg, eq_index - 1));
}

int env_find_index(char **envp, char *key)
{
    int i;
    int key_len;

    i = 0;
    key_len = ft_strlen(key);
    while (envp[i])
    {
        if (ft_strncmp(envp[i], key, key_len) == 0 && envp[i][key_len] == '=')
            return (i);
        i++;
    }
    return (-1);
}

int env_count(char **envp)
{
    int i;

    i = 0;
    if (!envp)
        return (0);
    while (envp[i])
        i++;
    return (i);
}
static char **env_add_or_replace(char **envp, char *new_entry, char *key)
{
    char **new_env;
    int index;
    int count;
    int i;

    index = env_find_index(envp, key);
    if (index != -1)
    {
        free(envp[index]);
        envp[index] = new_entry;
        return (envp);
    }
    count = env_count(envp);
    new_env = malloc(sizeof(char *) * (count + 2));
    if (!new_env)
    {
        free(new_entry);
        return (NULL);
    }
    i = 0;
    while (i < count)
    {
        new_env[i] = envp[i];
        i++;
    }
    new_env[i] = new_entry;
    new_env[i + 1] = NULL;
    free(envp);
    return (new_env);
}
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
    if (eq == -1)
    {
        printf("declare -x %s\n", entry);
        return ;
    }
    i = 0;
    printf("declare -x ");
    while (i < eq)
        printf("%c", entry[i++]);
    printf("=\"");
    i = eq + 1;
    while (entry[i])
    {
        if (entry[i] == '\\' || entry[i] == '"'
            || entry[i] == '$' || entry[i] == '`')
            printf("\\");
        printf("%c", entry[i]);
        i++;
    }
    printf("\"\n");
}

static int	print_export_no_args(t_shell *shell)
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

int ft_export(char **argv, t_shell *shell)
{
    int i;
    int eq_index;
    char *key;
    char *new_entry;
    char **envp_temp;
    int status;

    if (!argv[1])
        return (print_export_no_args(shell));
    i = 1;
    status = 0;
    while (argv[i])
    {
        eq_index = find_equal_sign(argv[i]);
        if (argv[i][0] == '-' && argv[i][1] != '\0')
        {
            fprintf(stderr, "export: `%s': not a valid identifier\n", argv[i]);
            return (2);
        }
        if (has_append_syntax(argv[i], eq_index))
        {
            i++;
            continue;
        }
        if (!is_valid_identifier(argv[i], eq_index))
        {
            fprintf(stderr, "export: `%s': not a valid identifier\n", argv[i]);
            status = 1;
            i++;
            continue;
        }
        if (eq_index == -1)
        {
            i++;
            continue;
        }
        key = ft_substr(argv[i], 0, eq_index);
        new_entry = ft_strdup(argv[i]);
        envp_temp = env_add_or_replace(shell->envp, new_entry, key);
        if (envp_temp)
            shell->envp = envp_temp;
        free(key);
        i++;
    }
    return (status);
}
