/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 18:49:30 by meel-war          #+#    #+#             */
/*   Updated: 2025/05/30 14:56:19 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	is_only_space_or_sign(char *str, int *error)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if ((str[i] == '+' || str[i] == '-') && str[i + 1] == '\0')
			*error = 1;
		if (str[i] != ' ' && str[i] != '\t')
			return ;
		i++;
	}
	*error = 1;
}

static unsigned long long	check_overflow(char *str, int *error, int i,
										unsigned long long result)
{
	unsigned long long	before;

	while (str[i] >= '0' && str[i] <= '9')
	{
		before = result;
		result = result * 10 + (str[i++] - '0');
		if (before > result)
		{
			*error = 1;
			return (result);
		}
	}
	if (str[i] != '\0')
		*error = 1;
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	return (result);
}

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
	result = check_overflow(str, error, i, result);
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
	exit_value = atoi_exit_code(token_ptr->next->str, &error);
	is_only_space_or_sign(token_ptr->next->str, &error);
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
	if (token_ptr->next->str[0] == '\0')
	{
		ft_putstr_fd("minishell: exit: : numeric argument required\n", 2);
		return (2);
	}
	return (exit_value);
}

int	ft_exit(t_shell *shell, t_token *token_ptr)
{
	int	exit_value;

	if (ft_strcmp(token_ptr->str, "exit") != 0)
		return (-1);
	ft_putstr_fd("exit\n", 1);
	if (!token_ptr->next)
		free_all(shell, shell->exit_status);
	exit_value = validate_exit_args(token_ptr);
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
