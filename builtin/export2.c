/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 16:21:23 by meel-war          #+#    #+#             */
/*   Updated: 2025/05/27 15:48:47 by meel-war         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

static char	**create_env_copy(char **env, int env_size, t_shell *shell)
{
	int		i;
	char	**new_sorted_env;

	i = 0;
	new_sorted_env = malloc(sizeof(char *) * (env_size + 1));
	if (!new_sorted_env)
		free_all(shell, 1);
	while (i < env_size)
	{
		new_sorted_env[i] = ft_strdup(env[i]);
		if (!new_sorted_env[i])
		{
			while (--i >= 0)
				free(new_sorted_env[i]);
			free(new_sorted_env);
			free_all(shell, 1);
		}
		i++;
	}
	new_sorted_env[env_size] = NULL;
	return (new_sorted_env);
}

static char	**merge_env_with_export(t_shell *shell, char **display_exp,
		int env_size, int export_size)
{
	int		i;
	int		j;
	char	**temp;

	temp = display_exp;
	i = 0;
	display_exp = malloc(sizeof(char *) * (env_size + export_size + 1));
	if (!display_exp)
		free_all(shell, 1);
	while (i < env_size)
	{
		display_exp[i] = temp[i];
		i++;
	}
	j = 0;
	while (j < export_size)
	{
		if (find_env_var(shell->data->new_env, shell->data->sorted_env[j]) ==
			-1)
		{
			display_exp[i] = safe_strdup(shell->data->sorted_env[j], shell);
			i++;
		}
		j++;
	}
	display_exp[i] = NULL;
	free(temp);
	return (display_exp);
}

static void	print_and_free(char **display_exp, int total_size)
{
	int	i;

	sort_env(display_exp, total_size);
	i = 0;
	while(display_exp[i])
		print_export_format(display_exp[i++]);
	i = 0;
	while (display_exp[i])
		free(display_exp[i++]);
	free(display_exp);
}

int	export_no_args(t_shell *shell)
{
	int		env_size;
	char	**display_exp;
	int		export_size;

	env_size = 0;
	while (shell->data->new_env[env_size])
		env_size++;
	export_size = 0;
	while (shell->data->sorted_env && shell->data->sorted_env[export_size])
		export_size++;
	display_exp = create_env_copy(shell->data->new_env, env_size, shell);
	if (export_size > 0)
		display_exp = merge_env_with_export(shell, display_exp, env_size,
				export_size);
	print_and_free(display_exp, env_size + export_size);
	return (1);
}
