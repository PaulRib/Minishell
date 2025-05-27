/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:31:06 by meel-war          #+#    #+#             */
/*   Updated: 2025/05/27 14:36:00 by meel-war         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

int	var_exists_in_sorted_export(char **sorted_env, char *var_name)
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
