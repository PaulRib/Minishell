/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 13:10:09 by meel-war          #+#    #+#             */
/*   Updated: 2025/04/25 14:13:05 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	print_env(t_shell *hub)
{
	int	i;

	i = 0;
	if(!hub->data->new_env[i])
		return(1);
	while (hub->data->new_env[i])
	{
		printf("%s\n", hub->data->new_env[i]);
		i++;
	}
	return(0);
}

char	**build_env(t_data *data)
{
	char	**new_env;
	char	*temp;

	new_env = malloc(2 * sizeof(char *));
	if (!new_env)
		return (NULL);
	new_env[0] = ft_strdup("SHLVL=");
	temp = ft_itoa(data->shlvl);
	new_env[0] = ft_strjoin("SHLVL=", temp);
	new_env[1] = NULL;
	free(temp);
	return (new_env);
}

char	**copy_env(char **env, t_data *data)
{
	int		i;
	int		j;
	char	**new_env;
	int		found_shlvl;
	char	*temp;

	i = 0;
	j = 0;
	found_shlvl = 0;
	while (env[i])
		i++;
	new_env = malloc((i + 1) * sizeof(char *));
	if (!new_env)
		return (NULL);
	while (j < i)
	{
		if (ft_strncmp(env[j], "SHLVL=", 6) == 0)
		{
			found_shlvl = 1;
			data->shlvl = ft_atoi(env[j] + 6) + 1;
			temp = ft_itoa(data->shlvl);
			new_env[j] = ft_strjoin("SHLVL=", temp);
			free(temp);
		}
		else
			new_env[j] = ft_strdup(env[j]);
		if (!new_env[j])
		{
			free_tab(new_env);
			return (NULL);
		}
		j++;
	}
	new_env[i] = NULL;
	return (new_env);
}
