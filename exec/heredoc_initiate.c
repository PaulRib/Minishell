/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_initiate.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 15:59:09 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/30 16:03:28 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	allocate_heredoc_eof(t_shell *shell)
{
	t_heredoc	*tmp;

	tmp = shell->heredoc;
	while (tmp)
	{
		tmp->eof_heredoc = malloc(sizeof(char *) * (tmp->nb_heredoc + 1));
		if (!tmp->eof_heredoc)
			free_all(shell, 1);
		tmp = tmp->next;
	}
}

static void	add_eof_to_heredoc(t_shell *shell, t_token *current, t_heredoc *tmp,
		int *i)
{
	char	*dup;

	if (current->type == END)
	{
		dup = ft_strdup(current->str);
		if (!dup)
			free_all(shell, 1);
		tmp->eof_heredoc[(*i)++] = dup;
	}
}

static void	fill_heredoc_eof(t_shell *shell)
{
	t_token		*current;
	t_heredoc	*tmp;
	int			i;

	current = shell->token;
	tmp = shell->heredoc;
	while (tmp)
	{
		i = 0;
		while (current)
		{
			add_eof_to_heredoc(shell, current, tmp, &i);
			if (current->type == PIPE && current->prev->type == FILE_OUT)
			{
				current = current->next;
				break ;
			}
			current = current->next;
		}
		tmp->eof_heredoc[i] = NULL;
		tmp = tmp->next;
	}
}

void	initiate_heredoc(t_shell *shell)
{
	t_token		*current;
	t_heredoc	*tmp;
	int			process;

	process = 0;
	current = shell->token;
	shell->heredoc = malloc(sizeof(t_heredoc));
	if (!shell->heredoc)
		free_all(shell, 1);
	ft_memset(shell->heredoc, 0, sizeof(t_heredoc));
	shell->heredoc->next = NULL;
	tmp = shell->heredoc;
	while (current)
	{
		check_current_type(current, tmp, process);
		if (current->type == PIPE && current->prev->type == FILE_OUT)
		{
			check_and_create(shell, current->next, tmp);
			if (tmp->next)
				tmp = tmp->next;
			process++;
		}
		current = current->next;
	}
}

void	stock_all_heredoc(t_shell *shell)
{
	allocate_heredoc_eof(shell);
	fill_heredoc_eof(shell);
}
