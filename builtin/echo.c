/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 14:15:41 by meel-war          #+#    #+#             */
/*   Updated: 2025/04/15 13:09:53 by meel-war         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_echo(t_token *token_ptr)
{
	t_token	*current;
	int		n_param;

	n_param = 0;
	current = token_ptr->next;
	while (current && current->type == WORD && ft_strcmp(current->str,
			"-n") == 0)
	{
		n_param = 1;
		current = current->next;
	}
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
	if (ft_strcmp(token_ptr->str, "echo") != 0)
		return (-1);
	return (ft_echo(token_ptr));
}