/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 14:15:41 by meel-war          #+#    #+#             */
/*   Updated: 2025/04/04 17:33:50 by meel-war         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int args_nb(char **temp)
{
	int		count;

	count = 0;
	while(temp[count])
		count++;
	return(count);
}

int	ft_echo(char **temp)
{
	int i;
	int n_param;

	i = 1;
	n_param = 0;
	if(args_nb(temp) > 1)
	{
		while(temp[i] && ft_strcmp(temp[i], "-n") == 0)
		{
			n_param = 1;
			i++;
		}
		while(temp[i])
		{
			ft_putstr_fd(temp[i], 1);
			if(temp[i + 1])
				write(1, " ", 1);
			i++;
		}
	}
	if(n_param == 0)
	{
		write(1, "\n", 1);
	}
}

static int check_echo(t_shell *shell, t_token *token_ptr)
{
	if(ft_strcmp(token_ptr->str, "echo") != 0)
		return (-1);
	return(ft_echo(token_ptr));
}