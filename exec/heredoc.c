/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 13:36:54 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/30 15:59:52 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	close_all_heredoc_fds(t_heredoc *heredoc_list)
{
	t_heredoc	*current_hd;

	current_hd = heredoc_list;
	while (current_hd)
	{
		if (current_hd->p_fd[0] > 0 && current_hd->p_fd[0] != -1)
		{
			close(current_hd->p_fd[0]);
			current_hd->p_fd[0] = -1;
		}
		if (current_hd->p_fd[1] > 0 && current_hd->p_fd[1] != -1)
		{
			close(current_hd->p_fd[1]);
			current_hd->p_fd[1] = -1;
		}
		current_hd = current_hd->next;
	}
}

int	create_heredoc_pipes(t_shell *shell)
{
	t_heredoc	*current;

	current = shell->heredoc;
	while (current)
	{
		current->p_fd[0] = -1;
		current->p_fd[1] = -1;
		if (current->hrd == true)
		{
			if (pipe(current->p_fd) == -1)
			{
				perror("minishell: pipe for heredoc");
				shell->exit_status = 1;
				close_all_heredoc_fds(shell->heredoc);
				return (1);
			}
		}
		current = current->next;
	}
	return (0);
}

int	handle_all_heredocs(t_shell *shell)
{
	int	process_status;

	if (!shell->heredoc)
		return (0);
	if (create_heredoc_pipes(shell) != 0)
		free_all(shell, 1);
	process_status = process_heredoc_inputs_loop(shell);
	if (process_status == 0 || process_status == 1)
		return (0);
	else
	{
		close_all_heredoc_fds(shell->heredoc);
		ft_free_heredoc(&shell);
		shell->heredoc = NULL;
		if (process_status == 2)
			return (130);
		else
			return (1);
	}
}
