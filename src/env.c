/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 13:10:09 by meel-war          #+#    #+#             */
/*   Updated: 2025/05/26 20:27:17 by meel-war         ###   ########.fr       */
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

char	**build_env(t_data *data)
{
	char	**new_env;
	char	*temp;
	char	pwd[PATH_MAX];

	new_env = malloc(4 * sizeof(char *));
	if (!new_env)
		return (NULL);
	temp = ft_itoa(data->shlvl);
	new_env[0] = ft_strjoin("SHLVL=", temp);
	free(temp);
	getcwd(pwd, PATH_MAX);
	new_env[1] = ft_strjoin("PWD=", pwd);
	new_env[2] = ft_strdup("_=/usr/bin/env");
	new_env[3] = NULL;
	return (new_env);
}

static int	fill_env(char **env, char **new_env, t_data *data, int size)
{
	int		j;
	char	*tmp;

	j = 0;
	while (j < size)
	{
		if (ft_strncmp(env[j], "SHLVL=", 6) == 0)
		{
			data->shlvl = ft_atoi(env[j] + 6) + 1;
			tmp = ft_itoa(data->shlvl);
			new_env[j] = ft_strjoin("SHLVL=", tmp);
			free(tmp);
		}
		else
			new_env[j] = ft_strdup(env[j]);
		if (!new_env[j])
		{
			return (0);
		}
		j++;
	}
	new_env[size] = NULL;
	return (1);
}

char	**copy_env(char **env, t_data *data)
{
	int		i;
	char	**new_env;

	i = 0;
	while (env[i])
		i++;
	new_env = malloc((i + 1) * sizeof(char *));
	if (!new_env)
		return (NULL);
	if (!fill_env(env, new_env, data, i))
	{
		free_tab(new_env);
		return (NULL);
	}
	return (new_env);
}

void	env_exists(char **env, t_shell *shell)
{
	if (!env || !env[0])
		shell->data->new_env = build_env(shell->data);
	else
		shell->data->new_env = copy_env(env, shell->data);
}
