/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:01:22 by pribolzi          #+#    #+#             */
/*   Updated: 2025/04/16 15:20:56 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	count_element(t_shell *shell)
{
	t_token	*current;

	current = shell->token;
	while (current->next)
	{
		if (current->type == PIPE)
			shell->count->nb_pipe++;
		if (current->type == REDIR_IN)
			shell->count->nb_redir_in++;
		if (current->type == REDIR_OUT)
			shell->count->nb_redir_out++;
		if (current->type == APPEND)
			shell->count->nb_append++;
		if (current->type == HEREDOC)
			shell->count->nb_heredoc++;
		current = current->next;
	}
}

void	exec_hub(t_shell *shell)
{
	count_element(shell);
}
