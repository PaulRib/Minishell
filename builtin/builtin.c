/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 13:12:19 by pribolzi          #+#    #+#             */
/*   Updated: 2025/06/05 17:11:58 by pribolzi         ###   ########.fr       */
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
		if (ft_exit(shell, token_ptr) != -1)
			return (1);
		token_ptr = token_ptr->next;
	}
	return (-1);
}

static void	builtin_fds(t_shell *shell, t_token *current, int type)
{
	if (shell->exec->fd_in[0] > 2)
	{
		dup2(shell->exec->fd_in[0], STDIN_FILENO);
		close(shell->exec->fd_in[0]);
	}
	if (shell->exec->fd_out[0] > 2)
	{
		dup2(shell->exec->fd_out[0], STDOUT_FILENO);
		close(shell->exec->fd_out[0]);
	}
	if (type == 1)
		check_cd(shell, current);
	if (type == 2)
		check_unset(shell, current);
	if (type == 3)
		check_export(shell, current);
}

static int	do_solo_redir(t_shell *shell, t_token *current, int type)
{
	int	save;
	int	save1;

	save = dup(STDOUT_FILENO);
	save1 = dup(STDIN_FILENO);
	if (shell->count->nb_redir_in > 0)
		open_infile(shell);
	if (shell->count->nb_redir_out > 0)
		open_outfile(shell);
	if (shell->count->nb_heredoc > 0)
	{
		initiate_heredoc(shell);
		stock_all_heredoc(shell);
		process_heredoc_inputs_loop(shell);
	}
	builtin_fds(shell, current, type);
	dup2(save1, STDIN_FILENO);
	dup2(save, STDOUT_FILENO);
	close(save1);
	close(save);
	if (type == 4)
		ft_exit(shell, current);
	return (1);
}

int	check_one_builtin(t_shell *shell)
{
	t_token	*current;

	current = shell->token;
	if (shell->exec->process == 1 && shell->count->nb_pipe == 0)
	{
		while (current && (current->type == REDIR_IN
				|| current->type == REDIR_OUT || current->type == HEREDOC
				|| current->type == APPEND))
			current = current->next;
		if (ft_strcmp(current->str, "cd") == 0)
			return (do_solo_redir(shell, current, 1));
		else if (ft_strcmp(current->str, "unset") == 0)
			return (do_solo_redir(shell, current, 2));
		else if (ft_strcmp(current->str, "export") == 0)
			return (do_solo_redir(shell, current, 3));
		else if (ft_strcmp(current->str, "exit") == 0)
			return (do_solo_redir(shell, current, 4));
	}
	return (0);
}

int	is_target_builtin(t_shell *shell, t_token *target)
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
	if (ft_exit(shell, target) != -1)
		return (1);
	return (-1);
}
