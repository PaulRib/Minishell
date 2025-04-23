/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:38:15 by meel-war          #+#    #+#             */
/*   Updated: 2025/04/17 16:24:36 by meel-war         ###   ########.fr       */
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
	while (var[len] && var[len] != '=')
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
	var_name = ft_substr(var, 0, equal_sign - var);
	var_value = ft_strdup(equal_sign + 1);
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
	while (token_ptr && token_ptr->type == WORD)
	{
		if (!export_var(shell->data, token_ptr->str))
			return (-1);
		token_ptr = token_ptr->next;
	}
	return (0);
}
