/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 13:10:09 by meel-war          #+#    #+#             */
/*   Updated: 2025/02/28 13:13:00 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char **copy_env(char **env)
{
	int i = 0;
	int j = 0;
	char **new_env;

	if(!env || !env[0])
		return(build_env(env));
	while(env[i])
		i++;
	new_env = malloc((i + 1) * sizeof(char *));
	if(!new_env)
		return (NULL);
	while(j < i)
	{
		new_env[j] = ft_strdup(env[j]);
		if(new_env[j])
		{
			free_tab(new_env);
			return(NULL);
		}
		j++;
	}
	new_env[i] = NULL;
	return(new_env);
}