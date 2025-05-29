/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 18:49:30 by meel-war          #+#    #+#             */
/*   Updated: 2025/05/29 18:58:39 by meel-war         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	atoi_exit_code(char *str, int *error)
{
	unsigned long long	result;
	int					i;
	int					sign;

	result = 0;
	i = 0;
	sign = 1;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		unsigned long long before = result;
		result = result * 10 + (str[i++] - '0');
		if (before > result) {
			*error = 1;
			return(0);
		}
	}
	if (str[i] != '\0')
		*error = 1;
	if ((sign == 1 && result > LONG_MAX) || (sign == -1
			&& result > (unsigned long long)LONG_MAX + 1))
		*error = 1;
	return ((int)((result * sign) % 256));
}

static int	validate_exit_args(t_token *token_ptr)
{
	int	error;
	int	exit_value;

	error = 0;
	if (!token_ptr->next)
		return (-1);
	exit_value = atoi_exit_code(token_ptr->next->str, &error);
	if (error)
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(token_ptr->next->str, 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		return (-2);
	}
	if (token_ptr->next && token_ptr->next->next)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	return (exit_value);
}

int	ft_exit(t_shell *shell, t_token *token_ptr)
{
	int	exit_value;

	if (ft_strcmp(token_ptr->str, "exit") != 0)
		return (-1);
	// ft_putstr_fd("exit\n", 1);
	exit_value = validate_exit_args(token_ptr);
	if (exit_value == -1)
		free_all(shell, shell->exit_status);
	if (exit_value == -2)
		free_all(shell, 2);
	if (exit_value == 1)
	{
		shell->exit_status = 1;
		return (1);
	}
	shell->exit_status = exit_value;
	free_all(shell, shell->exit_status);
	return (1);
}
