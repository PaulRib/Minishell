/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:38:15 by meel-war          #+#    #+#             */
/*   Updated: 2025/05/27 16:52:42 by meel-war         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	handle_plus_equal(t_shell *shell, char *var, char *equal_sign,
		t_export *xpr)
{
	char	*plus_sign;
	char	*old_value;
	char	*new_value;

	plus_sign = ft_strchr(var, '+');
	if (!plus_sign || plus_sign[1] != '=' || plus_sign >= equal_sign)
		return (0);
	xpr->var_name = ft_substr(var, 0, plus_sign - var);
	if (!xpr->var_name)
		free_all(shell, 1);
	xpr->var_value = safe_strdup(equal_sign + 1, shell);
	old_value = ft_get_env(shell->data->new_env, xpr->var_name);
	if (old_value)
	{
		new_value = safe_strjoin(old_value, xpr->var_value, shell, 0);
		if (!new_value)
			free(xpr->var_name);
		xpr->var_value = new_value;
	}
	return (1);
}

static int	add_to_export_list(t_data *data, char *var_name, t_shell *shell)
{
	int		i;
	int		env_size;
	char	**new_sorted_env;
	char	*var_copy;

	if (find_env_var(data->new_env, var_name) != -1)
		return (0);
	if (check_var_export(data->sorted_env, var_name))
		return (0);
	env_size = 0;
	while (data->sorted_env && data->sorted_env[env_size])
		env_size++;
	new_sorted_env = malloc(sizeof(char *) * (env_size + 2));
	if (!new_sorted_env)
		free_all(shell, 1);
	i = -1;
	while (++i < env_size)
		new_sorted_env[i] = data->sorted_env[i];
	var_copy = safe_strdup(var_name, shell);
	new_sorted_env[env_size] = var_copy;
	new_sorted_env[env_size + 1] = NULL;
	if (data->sorted_env)
		free(data->sorted_env);
	data->sorted_env = new_sorted_env;
	return (0);
}

static int	export_var(t_data *data, char *var, t_shell *shell)
{
	t_export	xpr;
	char		*equal_sign;

	xpr.var_name = NULL;
	xpr.var_value = NULL;
	if (!is_valid_identifier(var))
		return (-1);
	equal_sign = ft_strchr(var, '=');
	if (!equal_sign)
		return (add_to_export_list(data, var, shell), 0);
	if (!handle_plus_equal(shell, var, equal_sign, &xpr))
	{
		xpr.var_name = ft_substr(var, 0, equal_sign - var);
		if (!xpr.var_name)
			free_all(shell, 1);
		xpr.var_value = safe_strdup(equal_sign + 1, shell);
	}
	update_env_var(data, xpr.var_name, xpr.var_value);
	free(xpr.var_name);
	free(xpr.var_value);
	return (0);
}

int	check_export(t_shell *shell, t_token *token_ptr)
{
	if (ft_strcmp(token_ptr->str, "export") != 0)
		return (-1);
	if (token_ptr && !token_ptr->next)
	{
		if (!export_no_args(shell))
			return (-1);
		return (0);
	}
	token_ptr = token_ptr->next;
	while (token_ptr && (token_ptr->type == WORD || token_ptr->type == S_QUOTE
			|| token_ptr->type == D_QUOTE))
	{
		if (token_ptr->next)
		{
			if (token_ptr->next->type == S_QUOTE
				|| token_ptr->next->type == D_QUOTE)
				token_ptr->str = safe_strjoin(token_ptr->str,
						token_ptr->next->str, shell, 1);
		}
		if (!export_var(shell->data, token_ptr->str, shell))
			return (-1);
		token_ptr = token_ptr->next;
	}
	return (0);
}
