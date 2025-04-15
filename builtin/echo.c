/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 14:15:41 by meel-war          #+#    #+#             */
/*   Updated: 2025/04/15 16:11:13 by meel-war         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int ft_count(char *str)
{
	int i;
	int n;
	i = 0;
	n = 0;
	int flag = 0;
	
	while(str[i])
	{
		if(ft_strncmp(&str[i], "-n", 2) == 0 && flag == 0)
		{
			i++;
			n++;
		}
		else if(i > 4 && ft_strcmp(&str[i], "-n") != 0)
		{
			flag = 1;
			write(1, &str[i], 1);
		}
		i++;
	}
	return(n);
}

int	ft_echo(t_token *token_ptr)
{
	t_token	*current;
	int		n_param;

	n_param = 0;
	n_param = ft_count(token_ptr->str);
	current = token_ptr->next;
	while (current && current->type == WORD)
	{
		ft_putstr_fd(current->str, 1);
		if (current->next && current->next->type == WORD)
			write(1, " ", 1);
		current = current->next;
	}
	if (n_param == 0)
		write(1, "\n", 1);
	return (0);
}

int	check_echo(t_token *token_ptr)
{
	if (ft_strncmp(token_ptr->str, "echo", 4) != 0)
		return (-1);
	return (ft_echo(token_ptr));
}