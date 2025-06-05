/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_quotes_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 13:08:09 by pribolzi          #+#    #+#             */
/*   Updated: 2025/06/05 17:02:26 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	find_space(t_token *current, int i)
{
	if (i == 0)
	{
		while (current->str[i])
		{
			if (current->str[i] == ' ' || current->str[i] == '\t')
				return (i);
			i++;
		}
	}
	else
	{
		while (i > 0)
		{
			if (current->str[i] == ' ' || current->str[i] == '\t')
				return (i);
			i--;
		}
	}
	return (i);
}

int	join_check(t_token *current, int len)
{
	if (current->str && current->str[len] != '>'
		&& current->str[len] != '<' && current->str[len] != '|')
	{
		if (current->str && current->str[len] != ' '
			&& current->str[len] != '\t')
			return (1);
		else
			return (0);
	}
	return (0);
}
