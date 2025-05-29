/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 13:10:09 by meel-war          #+#    #+#             */
/*   Updated: 2025/05/29 18:47:57 by meel-war         ###   ########.fr       */
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

char	**build_env(t_shell *shell)
{
	char	**new_env;
	char	*temp;
	char	pwd[PATH_MAX];

	new_env = malloc(4 * sizeof(char *));
	if (!new_env)
		free_all(shell, 1);
	temp = ft_itoa(shell->data->shlvl);
	if (!temp)
	{
		free(new_env);
		free_all(shell, 1);
	}
	new_env[1] = ft_strjoin("SHLVL=", temp);
	free(temp);
	if (!getcwd(pwd, PATH_MAX))
		free_all(shell, 1);
	new_env[0] = ft_strjoin("PWD=", pwd);
	new_env[2] = ft_strdup("_=/usr/bin/env");
	if (!new_env[0] || !new_env[1] || !new_env[2])
	{
		free_tab(new_env);
		free_all(shell, 1);
	}
	return (new_env[3] = NULL, new_env);
}

static int	fill_env(char **env, char **new_env, t_shell *shell, int size)
{
	int		j;
	char	*tmp;

	j = 0;
	while (j < size)
	{
		if (ft_strncmp(env[j], "SHLVL=", 6) == 0)
		{
			shell->data->shlvl = ft_atoi(env[j] + 6) + 1;
			if (shell->data->shlvl > 1000)
				shell->data->shlvl = 1;
			tmp = ft_itoa(shell->data->shlvl);
			if (!tmp)
				free_all(shell, 1);
			new_env[j] = ft_strjoin("SHLVL=", tmp);
			free(tmp);
			if (!new_env[j])
				free_all(shell, 1);
		}
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
