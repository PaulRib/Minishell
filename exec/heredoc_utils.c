/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 15:39:31 by pribolzi          #+#    #+#             */
/*   Updated: 2025/06/05 18:09:56 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_heredoc_eof(t_shell *shell, char *delimiter)
{
	ft_putstr_fd("\nminishell: warning: ", 2);
	ft_putstr_fd("here-document delimited by end-of-file (wanted `", 2);
	if (delimiter)
		ft_putstr_fd(delimiter, 2);
	else
		ft_putstr_fd("unknown", 2);
	ft_putstr_fd("`)\n", 2);
	shell->exit_status = 0;
	return (1);
}

int	check_end(int *i, t_shell *shell, t_heredoc **curr)
{
	t_heredoc	*current;

	current = *curr;
	if (current->hrd == true)
		shell->exec->prev_fd[current->process] = current->p_fd[0];
	if (current->p_fd[1] > 2)
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

void	check_and_create(t_shell *shell, t_token *current, t_heredoc *tmp)
{
	t_heredoc	*new;

	while (current)
	{
		if (current->type == END)
		{
			new = malloc(sizeof(t_heredoc));
			if (!new)
				free_all(shell, 1);
			ft_memset(new, 0, sizeof(t_heredoc));
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = new;
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

void	ft_free_heredoc(t_shell **shell)
{
	t_heredoc	*hd_next;

	while ((*shell)->heredoc)
	{
		hd_next = (*shell)->heredoc->next;
		if ((*shell)->heredoc->eof_heredoc)
		{
			free_tab((*shell)->heredoc->eof_heredoc);
		}
		free((*shell)->heredoc);
		(*shell)->heredoc = hd_next;
	}
	(*shell)->heredoc = NULL;
}
