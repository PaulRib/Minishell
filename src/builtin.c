/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 13:12:19 by pribolzi          #+#    #+#             */
/*   Updated: 2025/04/16 15:38:10 by meel-war         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_builtin(t_shell *shell, t_token *token_list)
{
	t_token	*token_ptr;
	int		result;

	token_ptr = token_list;
	while (token_ptr)
	{
		if ((result = check_cd(shell, token_ptr)) != -1)
			return (result);
		if ((result = check_echo(token_ptr)) != -1)
			return (result);
		if ((result = check_pwd(shell, token_ptr)) != -1)
			return (result);
		if (!ft_strcmp(token_ptr->str, "env"))
			return (print_env(shell), 1);
		if (!ft_strcmp(token_ptr->str, "history"))
			return (ft_history(shell), 1);
		if ((result = check_unset(shell, token_ptr)) != -1)
			return (result);
		if ((result = check_export(shell, token_ptr)) != -1)
			return (result);
		token_ptr = token_ptr->next;
	}
	return (0);
}

/*
else if (!ft_strcmp(token_ptr->str, "exit"))
return (ft_exit("Exit\n"));
token_ptr = token_ptr->next;
*/
