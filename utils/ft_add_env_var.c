/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_add_env_var.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:53:00 by meel-war          #+#    #+#             */
/*   Updated: 2025/05/30 14:59:56 by meel-war         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	env_size(char **env)
{
	int	i;

	i = 0;
	while (env && env[i])
		i++;
	return (i);
}

static char	**copy_allocate_env(char **env, int size, t_shell *shell)
{
	char	**new_env;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new_env = malloc((size + 2) * sizeof(char *));
	if (!new_env)
		free_all(shell, 1);
	while (i < size)
	{
		new_env[i] = ft_strdup(env[i]);
		if (!new_env[i])
		{
			while (j < i)
			{
				free(new_env[j]);
				j++;
			}
			free(new_env);
			free_all(shell, 1);
		}
		i++;
	}
	return (new_env);
}

char	**add_env_var(char **env, char *new_var, t_shell *shell)
{
	char	**new_env;
	int		size;
	int		j;

	j = 0;
	size = env_size(env);
	new_env = copy_allocate_env(env, size, shell);
	new_env[size] = ft_strdup(new_var);
	if (!new_env[size])
	{
		while (j < size)
		{
			free(new_env[j]);
			j++;
		}
		free(new_env);
		free_all(shell, 1);
	}
	new_env[size + 1] = NULL;
	return (new_env);
}
