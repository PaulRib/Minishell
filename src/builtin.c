/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 13:12:19 by pribolzi          #+#    #+#             */
/*   Updated: 2025/04/09 16:57:30 by meel-war         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_builtin (t_shell *shell, t_token *token_list)
{
	t_token *token_ptr;
	int result;
	
	token_ptr = token_list;
	while (token_ptr)
	{
		result = check_cd(shell, token_ptr);
		if(result != -1)
			return(result);
		// result = check_echo(shell, token_ptr);
		// if(result != -1)
		// 	return(result);
		result = check_pwd(shell, token_ptr);
		if(result != -1)
		// 	return(result);
		// result = check_unset(shell, token_ptr);
		// if(result != -1)
			// return(result);
		token_ptr = token_ptr->next;
	}
	return (0);
}

/*
else if (!ft_strcmp(token_ptr->str, "history"))
return (ft_history(token_ptr));
else if (!ft_strcmp(token_ptr->str, "env"))
return (print_env(token_ptr));
else if (!ft_strcmp(token_ptr->str, "exit"))
return (ft_exit("Exit\n"));
token_ptr = token_ptr->next;
*/
