/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 13:36:54 by pribolzi          #+#    #+#             */
/*   Updated: 2025/06/03 15:24:34 by pribolzi         ###   ########.fr       */
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
