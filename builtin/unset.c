#include "../includes/minishell.h"

static int	find_env_var(char **env, char *var)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(var);
	while (env[i])
	{
		if (ft_strncmp(env[i], var, len) == 0 && env[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

static char	**remove_env_var(char **env, int index)
{
	int		i;
	int		j;
	int		env_len;
	char	**new_env;

	env_len = 0;
	while (env[env_len])
		env_len++;
	new_env = malloc(env_len * sizeof(char *));
	if (!new_env)
		return (NULL);
	i = 0;
	j = 0;
	while (env[i])
	{
		if (i != index)
		{
			new_env[j] = ft_strdup(env[i]);
			j++;
		}
		i++;
	}
	new_env[j] = NULL;
	return (new_env);
}

int	unset_handling(t_shell *shell)
{
	int var_index;
	char **new_env;
	char *var_name;

	var_name = shell->str;
	if (!var_name || !var_name[0])
		return (0);
	var_index = find_env_var(shell->data->new_env, var_name);
	if (var_index != -1)
	{
		new_env = remove_env_var(shell->data->new_env, var_index);
		if (new_env)
		{
			free_tab(shell->data->new_env);
			shell->data->new_env = new_env;
		}
	}
	return (0);
}