/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:38:15 by meel-war          #+#    #+#             */
/*   Updated: 2025/05/22 20:56:24 by meel-war         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	invalid_identifier(char *var)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(var, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	return (0);
}

static int	is_valid_identifier(char *var)
{
	int	i;
	int	len;

	len = 0;
	i = 1;
	if (!var || !*var)
		return (0);
	while (var[len] && var[len] != '=' && var[len] != '+')
		len++;
	if (len == 0 || (!ft_isalpha(var[0]) && var[0] != '_'))
		return (invalid_identifier(var));
	while (i < len)
	{
		if (!ft_isalnum(var[i]) && var[i] != '_')
			return (invalid_identifier(var));
		i++;
	}
	return (-1);
}
static int	handle_plus_equal(t_data *data, char *var, char *equal_sign,
		char **var_name, char **var_value)
{
	char	*plus_sign;
	char	*old_value;
	char	*new_value;

	plus_sign = ft_strchr(var, '+');
	if (!plus_sign || plus_sign[1] != '=' || plus_sign >= equal_sign)
		return (0);
	*var_name = ft_substr(var, 0, plus_sign - var);
	if(!*var_name)
		return(0); //free_all
	*var_value = ft_strdup(equal_sign + 1);
	if(!*var_value)
	{
		free(*var_name);
		//free_all
	}
	old_value = ft_get_env(data->new_env, *var_name);
	if (old_value)
	{
		new_value = ft_strjoin(old_value, *var_value);
		if(!new_value)
		{
			free(*var_name);
			free(*var_value);
			//free_all;
		}
		free(*var_value);
		*var_value = new_value;
	}
	return (1);
}

static int	export_var(t_data *data, char *var)
{
	char	*var_name;
	char	*var_value;
	char	*equal_sign;

	if (!is_valid_identifier(var))
		return (-1);
	equal_sign = ft_strchr(var, '=');
	if (!equal_sign)
		return (0);
	if (!handle_plus_equal(data, var, equal_sign, &var_name, &var_value))
	{
		var_name = ft_substr(var, 0, equal_sign - var);
		if (!var_name)
			return (-1); //free_all peut etre
		var_value = ft_strdup(equal_sign + 1);
		if(!var_value)
		{
			free(var_name);
			return(-1); //free_all
		}
	}
	update_env_var(data, var_name, var_value);
	free(var_name);
	free(var_value);
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
				token_ptr->str = ft_strjoin(token_ptr->str,
						token_ptr->next->str);
		}
		if (!export_var(shell->data, token_ptr->str))
			return (-1);
		token_ptr = token_ptr->next;
	}
	return (0);
}
