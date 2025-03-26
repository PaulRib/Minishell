/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:17:08 by meel-war          #+#    #+#             */
/*   Updated: 2025/03/26 15:26:24 by meel-war         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int	ft_unset(t_data *data, char *var_name)
{
	int var_index;
	char **new_env;

	if (!var_name || !var_name[0])
		return (0);
	var_index = find_env_var(data->new_env, var_name);
	if (var_index != -1)
	{
		new_env = remove_env_var(data->new_env, var_index);
		if (new_env)
		{
			free_tab(data->new_env);
			data->new_env = new_env;
		}
	}
	return (0);
}