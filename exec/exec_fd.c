/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_fd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 15:41:29 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/26 18:02:42 by meel-war         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	open_outfile(t_shell *shell)
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
						O_WRONLY | O_CREAT | O_TRUNC, 0777);
			else if (current->prev->type == APPEND)
				shell->exec->fd_out[i] = open(current->str,
						O_WRONLY | O_CREAT | O_APPEND, 0777);
		}
		if (shell->exec->fd_out[i] > 1 && current->type == PIPE)
			i++;
		current = current->next;
	}
}

int	open_infile(t_shell *shell)
{
	int		i;
	t_token	*current;

	current = shell->token;
	i = 0;
	while (current->next)
	{
		if (current->type == FILE_IN)
		{
			if (shell->exec->fd_in[i])
				close(shell->exec->fd_in[i]);
			shell->exec->fd_in[i] = open(current->str, O_RDONLY);
			if (shell->exec->fd_in[i] == -1)
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(current->str, 2);
				ft_putstr_fd(": No such file or directory\n", 2);
				return (-1);
			}
			if (current->type == HEREDOC)
				if (shell->exec->fd_in[i])
					close(shell->exec->fd_in[i]);
		}
		if (current->type == PIPE && (current->next->type == REDIR_IN
				|| current->next->type == HEREDOC))
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
			close(shell->exec->fd_in[i]);
		if (shell->exec->fd_out[i] > 2)
			close(shell->exec->fd_out[i]);
		if (shell->exec->prev_fd[i] > 2)
			close(shell->exec->prev_fd[i]);
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
			shell->exec->prev_fd[i] = 0;
		}
		i++;
	}
}

int	process_redirections(t_shell *shell)
{
	int	infile_status;

	if (shell->count->nb_redir_out > 0 || shell->count->nb_append > 0)
		open_outfile(shell);
	infile_status = 0;
	if (shell->count->nb_redir_in > 0)
		infile_status = open_infile(shell);
	if (infile_status == -1)
	{
		ft_free_exec(shell);
		ft_free_heredoc(shell);
		return (1);
	}
	return (0);
}
