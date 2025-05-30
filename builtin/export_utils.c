/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:31:06 by meel-war          #+#    #+#             */
/*   Updated: 2025/05/30 16:27:33 by meel-war         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_consecutive_plus(char *var)
{
	int	i;

	i = 0;
	if (var[i] == '+')
		i++;
	if (var[i] != '=')
		return (1);
	return (0);
}

int	is_valid_identifier(char *var)
{
	int	i;
	int	len;

	len = 0;
	i = 1;
	if (!var || !*var)
		return (0);
	while (var[len] && var[len] != '=' && var[len] != '+')
		len++;
	if(check_consecutive_plus(&var[len]))
		return(invalid_identifier_export(var));
	if (len == 0 || (!ft_isalpha(var[0]) && var[0] != '_'))
		return (invalid_identifier_export(var));
	while (i < len)
	{
		if (!ft_isalnum(var[i]) && var[i] != '_')
			return (invalid_identifier_export(var));
		i++;
	}
	return (-1);
}

int	check_var_export(char **sorted_env, char *var_name)
{
	int	i;
	int	len;

	if (!sorted_env || !var_name)
		return (0);
	len = ft_strlen(var_name);
	i = 0;
	while (sorted_env[i])
	{
		if (ft_strncmp(sorted_env[i], var_name, len) == 0
			&& (sorted_env[i][len] == '\0' || sorted_env[i][len] == '='))
			return (1);
		i++;
	}
	return (0);
}

void	print_export_value(char *env_var, int equal_sign)
{
	int	i;

	i = equal_sign + 1;
	ft_printf("=\"");
	while (env_var[i])
	{
		if (env_var[i] == '\"' || env_var[i] == '\\' || env_var[i] == '$')
			ft_printf("\\");
		ft_printf("%c", env_var[i]);
		i++;
	}
	ft_printf("\"");
}

void	print_export_format(char *env_var)
{
	int	i;
	int	equal_sign;

	equal_sign = -1;
	i = 0;
	while (env_var[i])
	{
		if (env_var[i] == '=')
		{
			equal_sign = i;
			break ;
		}
		i++;
	}
	ft_printf("declare -x ");
	i = 0;
	while (i < equal_sign || (equal_sign == -1 && env_var[i]))
		ft_printf("%c", env_var[i++]);
	if (equal_sign != -1)
		print_export_value(env_var, equal_sign);
	ft_printf("\n");
}

int	invalid_identifier_export(char *var)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(var, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	return (0);
}
