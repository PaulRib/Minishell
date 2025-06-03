/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 15:14:24 by pribolzi          #+#    #+#             */
/*   Updated: 2025/06/03 14:37:31 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	get_type(t_shell *shell)
{
	t_token		*tmp;

	tmp = shell->token;
	while (tmp)
	{
		if (tmp->type != S_QUOTE && tmp->type != D_QUOTE
			&& ft_strcmp(tmp->str, "|") == 0)
			tmp->type = PIPE;
		else if (tmp->type != S_QUOTE && tmp->type != D_QUOTE
			&& ft_strcmp(tmp->str, ">") == 0)
			tmp->type = REDIR_OUT;
		else if (tmp->type != S_QUOTE && tmp->type != D_QUOTE
			&& ft_strcmp(tmp->str, "<") == 0)
			tmp->type = REDIR_IN;
		else if (tmp->type != S_QUOTE && tmp->type != D_QUOTE
			&& ft_strcmp(tmp->str, ">>") == 0)
			tmp->type = APPEND;
		else if (tmp->type != S_QUOTE && tmp->type != D_QUOTE 
			&& ft_strcmp(tmp->str, "<<") == 0)
			tmp->type = HEREDOC;
		else if (tmp->type != S_QUOTE && tmp->type != D_QUOTE)
			tmp->type = WORD;
		tmp = tmp->next;
	}
}

void	insert_new_token(t_quote qte, t_token *new_token, t_token *current)
{
	new_token->next = current->next;
	if (current->next)
		current->next->prev = new_token;
	new_token->prev = current;
	current->next = new_token;
	if (qte.c == '"')
		new_token->type = D_QUOTE;
	else if (qte.c == '\'')
		new_token->type = S_QUOTE;
}

void	second_token(t_shell *shell)
{
	t_token	*tmp;

	tmp = shell->token;
	while (tmp->next)
	{
		if (tmp->type == REDIR_IN && tmp->next->type != PIPE)
			tmp->next->type = FILE_IN;
		if (tmp->type == REDIR_OUT && tmp->next->type != PIPE)
			tmp->next->type = FILE_OUT;
		if (tmp->type == HEREDOC && tmp->next->type != PIPE)
			tmp->next->type = END;
		if (tmp->type == APPEND && tmp->next->type != PIPE)
			tmp->next->type = FILE_OUT;
		if (tmp->type == PIPE && tmp->next->type != REDIR_IN
			&& tmp->next->type == WORD)
			tmp->next->type = CMD;
		tmp = tmp->next;
	}
}
