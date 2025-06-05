/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:17:08 by meel-war          #+#    #+#             */
/*   Updated: 2025/06/05 16:44:15 by meel-war         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	**remove_env_var(char **env, int index, t_shell *shell)
{
	int		i;
	int		j;
	int		env_len;
	char	**new_env;

	env_len = 0;
	while (env[env_len])
		env_len++;
	new_env = malloc((env_len) * sizeof(char *));
	if (!new_env)
		free_all(shell, 1);
	i = 0;
	j = 0;
	while (env[i])
	{
		if (i != index)
		{
			new_env[j] = safe_strdup(env[i], shell);
			j++;
		}
		i++;
	}
	new_env[j] = NULL;
	return (new_env);
}

static void	update_var_tab(char ***tab, int index, t_shell *shell)
{
	char	**new_tab;

	if (!tab || !(*tab) || index == -1)
		return ;
	new_tab = remove_env_var(*tab, index, shell);
	{
		if (new_tab)
		{
			free_tab(*tab);
			*tab = new_tab;
		}
	}
}

int	ft_unset(t_data *data, char *var_name, t_shell *shell)
{
	int	var_index;
	int	export_index;

	if (!var_name || !var_name[0])
		return (0);
	var_index = find_env_var(data->new_env, var_name);
	export_index = -1;
	if (data->sorted_env)
		export_index = find_exp_var(data->sorted_env, var_name);
	update_var_tab(&data->sorted_env, export_index, shell);
	update_var_tab(&data->new_env, var_index, shell);
	return (0);
}

int	check_unset(t_shell *shell, t_token *token_ptr)
{
	if (ft_strcmp(token_ptr->str, "unset") != 0)
		return (-1);
	if (!token_ptr->next)
		return (0);
	while (token_ptr && token_ptr->next && token_ptr->next->type != PIPE)
	{
		ft_unset(shell->data, token_ptr->next->str, shell);
		token_ptr = token_ptr->next;
	}
	return (0);
}
