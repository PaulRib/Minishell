/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 13:12:19 by pribolzi          #+#    #+#             */
/*   Updated: 2025/03/26 15:27:20 by meel-war         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


// int	ft_exit(char *error)
// {
// 	ft_putstr_fd(error, 2);
// 	ft_free_all()
// 	exit(1);
// }

int	is_builtin (t_shell *shell, t_token *token_list)
{
	t_token *token_ptr;
	
	token_ptr = token_list;
	while (token_ptr)
	{
		if (!ft_strcmp(token_ptr->str, "echo"))
			return (ft_echo(token_ptr));
		else if (!ft_strcmp(token_ptr->str, "cd"))
			return (ft_handling(token_ptr));
		else if (!ft_strcmp(token_ptr->str, "pwd"))
			return (ft_pwd());
		//else if (!ft_strcmp(token_ptr->str, "export"))
			// return (fonction export);
		else if (!ft_strcmp(token_ptr->str, "unset"))
		{
			if(token_ptr->next)
				return (ft_unset(shell->data, token_ptr->next->str));
			else
			{
				ft_putstr_fd("unset: not enough arguments\n", 2);
				return (1);
			}
		}
		else if (!ft_strcmp(token_ptr->str, "history"))
			return (ft_history(token_ptr));
		else if (!ft_strcmp(token_ptr->str, "env"))
			return (print_env(token_ptr));
		else if (!ft_strcmp(token_ptr->str, "exit"))
			return (ft_exit("Exit\n"));
		token_ptr = token_ptr->next;
	}
	return (0);
}