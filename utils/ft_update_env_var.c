/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_update_env_var.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:39:34 by meel-war          #+#    #+#             */
/*   Updated: 2025/04/04 17:37:25 by meel-war         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	update_env_var(t_data *data, char *var_name, char *var_value)
{
	int var_index;
	char *new_var;
	char **new_env;

	new_var = ft_strjoin(var_name, "=");
	new_var = ft_strjoin(new_var, var_value);

	var_index = find_env_var(data->new_env, var_name);
	if (var_index != -1)
	{
		free(data->new_env[var_index]);
		data->new_env[var_index = new_var];
	}
	else
	{
		new_env = add_env_var(data->new_env, new_var);
		free_tab(data->new_env);
		data->new_env = new_env;
		free(new_var);
	}
	return (0);
}
