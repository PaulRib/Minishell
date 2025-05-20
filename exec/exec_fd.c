/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_fd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 15:41:29 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/19 17:23:04 by meel-war         ###   ########.fr       */
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
				close (shell->exec->fd_out[i]);
			if (current->prev->type == REDIR_OUT)
				shell->exec->fd_out[i] = open(current->str, O_WRONLY
						| O_CREAT | O_TRUNC, 0777);
			else if (current->prev->type == APPEND)
				shell->exec->fd_out[i] = open(current->str, O_WRONLY
						| O_CREAT | O_APPEND, 0777);
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
				close (shell->exec->fd_in[i]);
			shell->exec->fd_in[i] = open(current->str, O_RDONLY);
			if (shell->exec->fd_in[i] == -1)
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(current->str, 2);
				ft_putstr_fd(": No such file or directory", 2);
				return (-1);
			}
			if (current->type == HEREDOC)
				shell->exec->fd_in[i] = 0;
		}
		if (current->type == PIPE && (current->next->type == REDIR_IN
				|| current->next->type == HEREDOC))
			i++;
		current = current->next;
	}
	return (0);
}

void close_fd_exec(t_shell *shell)
{
	int	i;
	
	i = 0;
	while (i < shell->exec->process)
	{
		if (shell->exec->fd_in[i] != 0)
			close(shell->exec->fd_in[i]);
		if (shell->exec->fd_out[i] != 1)
			close(shell->exec->fd_out[i]);
		i++;
	}
}
