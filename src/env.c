/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 13:10:09 by meel-war          #+#    #+#             */
/*   Updated: 2025/02/28 14:28:29 by meel-war         ###   ########.fr       */
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
char **build_env(char **env)
{
	static int SHLVL = 0;
	char **new_env;
	char *temp;
	
	SHLVL++;
	new_env = malloc(2 * sizeof(char *));
	if(!new_env)
		return(NULL);
	new_env[0] = ft_strdup("SHLVL=");
	temp = ft_itoa(SHLVL);
	ft_strjoin(new_env[0], temp);
	new_env[1] = NULL;
	free(temp);
	return(new_env);
}