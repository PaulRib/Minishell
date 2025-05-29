/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:17:08 by meel-war          #+#    #+#             */
/*   Updated: 2025/05/29 14:22:48 by pribolzi         ###   ########.fr       */
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
			new_env[j] = ft_strdup(env[i]);
			j++;
		}
		i++;
	}
	new_env[j] = NULL;
	return (new_env);
}

int	ft_unset(t_data *data, char *var_name, t_shell *shell)
{
	int		var_index;
	char	**new_env;

	if (!var_name || !var_name[0])
		return (0);
	var_index = find_env_var(data->new_env, var_name);
	if (var_index != -1)
	{
		new_env = remove_env_var(data->new_env, var_index, shell);
		if (new_env)
		{
			free_tab(data->new_env);
			data->new_env = new_env;
		}
	}
	return (0);
}

int	check_unset(t_shell *shell, t_token *token_ptr)
{
	if (ft_strcmp(token_ptr->str, "unset") != 0)
		return (-1);
	if (!token_ptr->next)
		return (0);
	return (ft_unset(shell->data, token_ptr->next->str, shell));
}
