/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 16:21:23 by meel-war          #+#    #+#             */
/*   Updated: 2025/04/30 15:11:33 by meel-war         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	print_export_value(char *env_var, int equal_sign)
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

static void	print_export_format(char *env_var)
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

static void	sort_env(char **sorted_env, int env_size)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (i < env_size - 1)
	{
		j = 0;
		while (j < env_size - i - 1)
		{
			if (ft_strcmp(sorted_env[j], sorted_env[j + 1]) > 0)
			{
				tmp = sorted_env[j];
				sorted_env[j] = sorted_env[j + 1];
				sorted_env[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

static char	**create_env_copy(char **env, int env_size)
{
	char	**sorted_env;
	int		i;

	i = 0;
	sorted_env = malloc(sizeof(char *) * (env_size + 1));
	if (!sorted_env)
		return (NULL);
	while (i < env_size)
	{
		sorted_env[i] = ft_strdup(env[i]);
		if (!sorted_env[i])
		{
			while (--i >= 0)
				free(sorted_env);
			free(sorted_env);
			return (NULL);
		}
		i++;
	}
	sorted_env[env_size] = NULL;
	return (sorted_env);
}

int	export_no_args(t_shell *shell)
{
	char	**sorted_env;
	int		env_size;
	int		i;

	env_size = 0;
	i = 0;
	while (shell->data->new_env[env_size])
		env_size++;
	sorted_env = create_env_copy(shell->data->new_env, env_size);
	if (!sorted_env)
		return (0);
	sort_env(sorted_env, env_size);
	while (sorted_env[i])
		print_export_format(sorted_env[i++]);
	i = 0;
	while (sorted_env[i])
		free(sorted_env[i++]);
	free(sorted_env);
	return (1);
}
