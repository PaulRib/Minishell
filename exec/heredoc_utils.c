/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 15:39:31 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/09 17:52:48 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_end(int *i, t_shell *shell, t_heredoc **curr)
{
	t_heredoc	*current;

	current = *curr;
	if (current->hrd == true)
		shell->exec->prev_fd[current->process] = current->p_fd[0];
	close(current->p_fd[1]);
	if (current->next)
	{
		(*i) = 0;
		*curr = current->next;
		return (0);
	}
	else
		return (-1);
}

void	create_new_hrd(t_shell *shell, t_heredoc *tmp)
{
	t_heredoc	*new;

	(void)shell;
	new = malloc(sizeof(t_heredoc));
	if (!new)
		return ;
	ft_memset(new, 0, sizeof(t_heredoc));
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

void	check_and_create(t_shell *shell, t_token *current, t_heredoc *tmp)
{
	while (current)
	{
		if (current->type == END)
		{
			create_new_hrd(shell, tmp);
			return ;
		}
		if (current->type == PIPE && (current->next->type == REDIR_IN
				|| current->next->type == HEREDOC))
			return ;
		current = current->next;
	}
}

void	check_current_type(t_token *current, t_heredoc *tmp, int process)
{
	if (current->type == FILE_IN || current->type == REDIR_IN)
	{
		if (tmp->process == process)
			tmp->hrd = false;
	}
	if (current->type == END)
	{
		tmp->hrd = true;
		tmp->nb_heredoc++;
		tmp->process = process;
	}
}

void	ft_free_heredoc(t_shell *shell)
{
	t_heredoc	*hd_next;

	while (shell->heredoc)
	{
		hd_next = shell->heredoc->next;
		if (shell->heredoc->eof_heredoc)
			free_tab(shell->heredoc->eof_heredoc);
		free(shell->heredoc);
		shell->heredoc = hd_next;
	}
}
