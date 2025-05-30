/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 13:10:09 by meel-war          #+#    #+#             */
/*   Updated: 2025/05/30 16:27:53 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	print_env(t_shell *hub)
{
	int	i;

	i = 0;
	if (!hub->data->new_env[i])
		return (1);
	while (hub->data->new_env[i])
	{
		printf("%s\n", hub->data->new_env[i]);
		i++;
	}
	return (1);
}

static char	*handle_shlvl(char *env_var, t_shell *shell)
{
	char	*tmp;
	char	*shlvl_str;
	char	*result;

	shell->data->shlvl = ft_atoi(env_var + 6) + 1;
	if (shell->data->shlvl >= 999)
	{
		ft_putstr_fd("bash: warning: shell level ", 2);
		shlvl_str = ft_itoa(shell->data->shlvl);
		ft_putstr_fd(shlvl_str, 2);
		free(shlvl_str);
		ft_putstr_fd(" too high, resetting to 1\n", 2);
		shell->data->shlvl = 1;
	}
	tmp = ft_itoa(shell->data->shlvl);
	if (!tmp)
		free_all(shell, 1);
	result = ft_strjoin("SHLVL=", tmp);
	free(tmp);
	if (!result)
		free_all(shell, 1);
	return (result);
}

static int	fill_env(char **env, char **new_env, t_shell *shell, int size)
{
	int		j;

	j = 0;
	while (j < size)
	{
		if (ft_strncmp(env[j], "SHLVL=", 6) == 0)
			new_env[j] = handle_shlvl(env[j], shell);
		else
			new_env[j] = ft_strdup(env[j]);
		if (!new_env[j])
			free_all(shell, 1);
		j++;
	}
	new_env[size] = NULL;
	return (1);
}

char	**copy_env(char **env, t_shell *shell)
{
	int		i;
	char	**new_env;

	i = 0;
	while (env[i])
		i++;
	new_env = malloc((i + 1) * sizeof(char *));
	if (!new_env)
		free_all(shell, 1);
	if (!fill_env(env, new_env, shell, i))
	{
		free_tab(new_env);
		free_all(shell, 1);
	}
	return (new_env);
}

void	env_exists(char **env, t_shell *shell)
{
	if (!env || !env[0])
		shell->data->new_env = build_env(shell);
	else
		shell->data->new_env = copy_env(env, shell);
}
