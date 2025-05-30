/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_quotes_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 13:08:09 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/30 14:58:45 by pribolzi         ###   ########.fr       */
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

t_token	*change_after(t_shell *shell, t_token *current, int end)
{
	int		len;
	char	*tmp;
	t_token	*next_token;

	len = ft_strlen(current->str);
	next_token = current->next;
	if (len > end + 1)
	{
		tmp = ft_substr(current->str, end + 1, len - end - 1);
		if (!tmp)
			free_all(shell, 1);
		free(current->str);
		current->str = tmp;
		return (current);
	}
	if (current->prev)
		current->prev->next = current->next;
	else
		shell->token = current->next;
	if (current->next)
		current->next->prev = current->prev;
	free(current->str);
	free(current);
	return (next_token);
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
