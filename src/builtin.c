/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 13:12:19 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/20 18:45:56 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_simple_builtin(t_shell *shell)
{
	t_token	*token_ptr;

	token_ptr = shell->token;
	while (token_ptr)
	{
		if (check_cd(shell, token_ptr) != -1)
			return (1);
		if (check_echo(token_ptr) != -1)
			return (1);
		if (check_pwd(shell, token_ptr) != -1)
			return (1);
		if (!ft_strcmp(token_ptr->str, "env"))
			return (print_env(shell), 1);
		if (!ft_strcmp(token_ptr->str, "history"))
			return (ft_history(shell), 1);
		if (check_unset(shell, token_ptr) != -1)
			return (1);
		if (check_export(shell, token_ptr) != -1)
			return (1);
		token_ptr = token_ptr->next;
	}
	return (-1);
}

int check_one_builtin(t_shell *shell)
{
	t_token	*current;

	current = shell->token;
	if (shell->exec->process == 1 && shell->count->nb_pipe == 0)
	{
		while (current)
		{
			if (ft_strcmp(current->str, "cd") == 0)
			{
				if (shell->count->nb_redir_in > 0)
					open_infile(shell);
				return (check_cd(shell, current));
			}
			else if (ft_strcmp(current->str, "unset") == 0)
			{
				if (shell->count->nb_redir_in > 0)
					open_infile(shell);
				return (check_unset(shell, current));
			}
			else if (ft_strcmp(current->str, "export") == 0)
			{
				if (shell->count->nb_redir_in > 0)
					open_infile(shell);
				return (check_export(shell, current));
			}
			current = current->next;
		}
	}
	return (0);
}

int is_target_builtin(t_shell *shell, t_token *target)
{
	if (check_cd(shell, target) != -1)
		return (1);
	if (check_echo(target) != -1)
		return (1);
	if (check_pwd(shell, target) != -1)
		return (1);
	if (!ft_strcmp(target->str, "env"))
		return (print_env(shell), 1);
	if (!ft_strcmp(target->str, "history"))
		return (ft_history(shell), 1);
	if (check_unset(shell, target) != -1)
		return (1);
	if (check_export(shell, target) != -1)
		return (1);
	return (-1);
}
