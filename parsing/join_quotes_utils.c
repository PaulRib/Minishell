/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_quotes_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 13:08:09 by pribolzi          #+#    #+#             */
/*   Updated: 2025/06/09 12:32:21 by pribolzi         ###   ########.fr       */
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
		if (len == 0)
			return (1);
		if (current->str && current->str[len] != ' '
			&& current->str[len] != '\t')
			return (1);
		else
			return (0);
	}
	return (0);
}

void	verify_space(t_shell *shell)
{
	t_token	*current;
	int		len;

	current = shell->token;
	while (current)
	{
		if (current->last_space == false)
		{
			len = ft_strlen(current->str);
			if (len > 0)
			{
				if (current->str[len - 1] == ' ' || current->str[len
						- 1] == '\t')
					current->last_space = true;
			}
		}
		current = current->next;
	}
}
