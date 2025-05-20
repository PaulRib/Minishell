/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 15:14:24 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/20 17:59:06 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	get_type(t_shell *shell)
{
	int			i;
	t_token		*tmp;

	tmp = shell->token;
	i = 0;
	while (tmp)
	{
		if (ft_strcmp(tmp->str, "|") == 0)
			tmp->type = PIPE;
		else if (ft_strcmp(tmp->str, ">") == 0)
			tmp->type = REDIR_OUT;
		else if (ft_strcmp(tmp->str, "<") == 0)
			tmp->type = REDIR_IN;
		else if (ft_strcmp(tmp->str, ">>") == 0)
			tmp->type = APPEND;
		else if (ft_strcmp(tmp->str, "<<") == 0)
			tmp->type = HEREDOC;
		else if (tmp->type != S_QUOTE && tmp->type != D_QUOTE)
			tmp->type = WORD;
		tmp = tmp->next;
	}
}

void	second_token(t_shell *shell)
{
	t_token	*tmp;

	tmp = shell->token;
	while (tmp->next)
	{
		if (tmp->type == REDIR_IN)
			tmp->next->type = FILE_IN;
		if (tmp->type == REDIR_OUT)
			tmp->next->type = FILE_OUT;
		if (tmp->type == HEREDOC)
			tmp->next->type = END;
		if (tmp->type == APPEND)
			tmp->next->type = FILE_OUT;
		// if (tmp->type == FILE_IN && tmp->next->type != REDIR_IN
		// 	&& tmp->next->type != HEREDOC)
		// 	tmp->next->type = CMD;
		if (tmp->type == PIPE && tmp->next->type != REDIR_IN
			&& tmp->next->type == WORD)
			tmp->next->type = CMD;
		tmp = tmp->next;
	}
}
