/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 15:14:24 by pribolzi          #+#    #+#             */
/*   Updated: 2025/03/12 15:41:27 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	get_type(t_token *token)
{
	int	i;

	i = 0;
	while (token->str[i])
	{
		if (ft_strcmp(token->str[i], "|") == 0)
			token->type[i] = PIPE;
		else if (ft_strcmp(token->str[i], ">") == 0)
			token->type[i] = REDIR_IN;
		else if (ft_strcmp(token->str[i], "<") == 0)
			token->type[i] = REDIR_OUT;
		else if (ft_strcmp(token->str[i], ">>") == 0)
			token->type[i] = APPEND;
		else if (ft_strcmp(token->str[i], "<<") == 0)
			token->type[i] = HEREDOC;
		else
			token->type[i] = WORD;
		i++;
	}
}

// void osef(t_token *token)
// {
// 	int i;

// 	i = 0;
// 	while (token->str[i])
// 	{
// 		if (token->type[i] == REDIR_OUT)
// 			token->type[i + 1] == FD_OUT
// 		if (token->type[i + 1] == FD_OUT && token->type[i + 2] != PIPE)
// 			token->type[i + 2] == CMD
// 		if (token->type[i] == REDIR_IN)
// 		{
// 			token->type[i + 1] == FD_IN
// 			token->type[i - 1] == CMD
// 		}
// 	}
// }
