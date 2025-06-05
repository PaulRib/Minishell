/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_fd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 15:41:29 by pribolzi          #+#    #+#             */
/*   Updated: 2025/06/03 17:26:01 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	open_outfile(t_shell *shell)
{
	int		i;
	t_token	*current;

	current = shell->token;
	i = 0;
	while (current)
	{
		if (current->type == FILE_OUT)
		{
			if (shell->exec->fd_out[i] != 1)
				close(shell->exec->fd_out[i]);
			if (current->prev->type == REDIR_OUT)
				shell->exec->fd_out[i] = open(current->str,
						O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC, 0777);
			else if (current->prev->type == APPEND)
				shell->exec->fd_out[i] = open(current->str,
						O_WRONLY | O_CREAT | O_APPEND | O_CLOEXEC, 0777);
			if (shell->exec->fd_out[i] == -1)
				return(verify_access_fd(current, 1));
		}
		if (shell->exec->fd_out[i] > 1 && current->type == PIPE)
			i++;
		current = current->next;
	}
	return (0);
}

int	open_infile(t_shell *shell)
{
	int		i;
	t_token	*current;

	current = shell->token;
	i = 0;
	while (current)
	{
		if (current->type == FILE_IN)
		{
			if (shell->exec->fd_in[i])
				close(shell->exec->fd_in[i]);
			shell->exec->fd_in[i] = open(current->str, O_RDONLY | O_CLOEXEC);
			if (shell->exec->fd_in[i] == -1)
				return (verify_access_fd(current, 0));
			if (current->type == HEREDOC)
				if (shell->exec->fd_in[i])
					close(shell->exec->fd_in[i]);
		}
		if (current->type == PIPE && current->prev->type == FILE_OUT)
			i++;
		current = current->next;
	}
	return (0);
}

void	close_fd_exec(t_shell *shell)
{
	int	i;

	i = 0;
	while (i < shell->exec->process)
	{
		if (shell->exec->fd_in[i] > 2)
		{
			close(shell->exec->fd_in[i]);
			shell->exec->fd_in[i] = -1;
		}
		if (shell->exec->fd_out[i] > 2)
		{
			close(shell->exec->fd_out[i]);
			shell->exec->fd_out[i] = -1;
		}
		if (shell->exec->prev_fd[i] > 2)
		{
			close(shell->exec->prev_fd[i]);
			shell->exec->prev_fd[i] = -1;
		}
		i++;
	}
}

void	setup_heredoc_fds(t_shell *shell)
{
	int	i;

	i = 0;
	while (i < shell->exec->process)
	{
		if (shell->exec->prev_fd[i] > 2)
		{
			shell->exec->fd_in[i] = shell->exec->prev_fd[i];
			shell->exec->prev_fd[i] = -1;
		}
		i++;
	}
}

void	process_redirections(t_shell *shell)
{
	int in_status;
	int out_status;

	in_status = 0;
	out_status = 0;
	if (shell->count->nb_redir_in > 0)
		in_status = open_infile(shell);
	if (shell->count->nb_redir_out > 0 || shell->count->nb_append > 0)
		out_status = open_outfile(shell);
	if (in_status == 1 || out_status == 1)
		free_all(shell, 1);
}
