/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_update_env_var.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:39:34 by meel-war          #+#    #+#             */
/*   Updated: 2025/05/30 16:02:13 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	update_env_var(t_shell *shell, char *var_name, char *var_value)
int	update_env_var(t_shell *shell, char *var_name, char *var_value)
{
	int		var_index;
	char	*new_var;
	char	*tmp;
	char	**new_env;

	tmp = safe_strjoin(var_name, "=", shell, 0);
	new_var = safe_strjoin(tmp, var_value, shell, 1);
	var_index = find_env_var(shell->data->new_env, var_name);
	if (var_index != -1)
	{
		free(shell->data->new_env[var_index]);
		shell->data->new_env[var_index] = new_var;
	}
	else
	{
		new_env = add_env_var(shell->data->new_env, new_var, shell);
		free_tab(shell->data->new_env);
		shell->data->new_env = new_env;
		free(new_var);
	}
	return (0);
}
