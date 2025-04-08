/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:56:31 by pribolzi          #+#    #+#             */
/*   Updated: 2025/04/08 17:55:25 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_closed(t_token *current, int i, char c)
{
	while (current->str[i])
	{
		if (current->str[i] == c)
			return (i);
		i++;
	}
	return (0);
}

int	is_space(t_token *after)
{
	int	i;

	i = 0;
	while (after->str[i])
	{
		if (after->str[i] != ' ' && after->str[i] != '\n'
			&& after->str[i] != '\t')
			return (1);
		i++;
	}
	return (0);
}

int	is_empty(t_token *current, int start)
{
	int	i;

	i = 0;
	while (current->str[i] && i < start)
	{
		if (current->str[i] != ' ' && current->str[i] != '\n'
			&& current->str[i] != '\t')
			return (1);
		i++;
	}
	return (0);
}

void	empty_quote_before(t_token *current, t_token *new, int end, char c)
{
	if (c == '"')
		current->type = D_QUOTE;
	else if (c == '\'')
		current->type = S_QUOTE;
	if (current->str[end + 1])
		after_mult_quote(current, end + 1);
	free(current->str);
	current->str = new->str;
	free(new);
}

void	before_quote(t_token *current, int start)
{
	char	*rest;

	while (start > 0 && (current->str[start] == ' '
			|| current->str[start] == '\t' || current->str[start] == '\n'))
		start--;
	if (start > 0)
	{
		rest = ft_substr(current->str, 0, start - 1);
		free(current->str);
		current->str = rest;
	}
}
