/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 13:12:19 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/12 16:02:39 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_builtin(t_shell *shell)
{
	t_token	*token_ptr;

	token_ptr = shell->token;
	while (token_ptr)
	{
		if (check_cd(shell, token_ptr) != -1)
			return (-1);
		if (check_echo(token_ptr) != -1)
			return (-1);
		if (check_pwd(shell, token_ptr) != -1)
			return (-1);
		if (!ft_strcmp(token_ptr->str, "env"))
			return (print_env(shell), 1);
		if (!ft_strcmp(token_ptr->str, "history"))
			return (ft_history(shell), 1);
		if (check_unset(shell, token_ptr) != -1)
			return (-1);
		if (check_export(shell, token_ptr) != -1)
			return (-1);
		token_ptr = token_ptr->next;
	}
	return (0);
}

/*
else if (!ft_strcmp(token_ptr->str, "exit"))
return (ft_exit("Exit\n"));
token_ptr = token_ptr->next;
*/
