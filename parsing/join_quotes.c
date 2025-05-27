/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_quotes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 11:44:29 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/27 14:24:10 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	find_space(t_token *current, int i)
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

static void	change_after(t_shell *shell, t_token *current, int end)
{
	int		len;
	char	*tmp;

	len = ft_strlen(current->str);
	if (len != end)
	{
		tmp = ft_substr(current->str, end + 1, len - end);
		if (!tmp)
			free_all(shell, 1);
		free(current->str);
		current->str = tmp;
	}
	else
	{
		current->prev->next = current->next;
		if (current->next)
			current->next->prev = current->prev;
		free(current->str);
		free(current);
		current = NULL;
	}
}

static void	fix_quote_before(t_shell *shell, t_token *current)
{
	char	*final;
	int		end;
	char	*tmp;

	end = find_space(current, 0);
	tmp = ft_substr(current->str, 0, end);
	if (!tmp)
		free_all(shell, 1);
	final = ft_strjoin(current->prev->str, tmp);
	free(tmp);
	if (!final)
		free_all(shell, 1);
	free(current->prev->str);
	current->prev->str = final;
	change_after(shell, current, end);
}

static void fix_quote_after(t_shell *shell, t_token *current)
{
	char *final;
	char *tmp;
	int start;

	start = find_space(current, ft_strlen(current->str) - 1);
	tmp = ft_substr(current->str, start + 1, ft_strlen(current->str) - start + 1);
	if (!tmp)
		free_all(shell, 1);
	final = ft_strjoin(tmp, current->next->str);
	free(tmp);
	if (!final)
		free_all(shell, 1);
	free(current->next->str);
	current->next->str = final;
	tmp = ft_substr(current->str, 0, start);
	if (!tmp)
		free_all(shell, 1);
	free(current->str);
	current->str = tmp;
}

void	join_quote(t_shell *shell)
{
	t_token	*current;

	current = shell->token;
	while (current)
	{
		if (current->prev)
			if (current->prev->type == S_QUOTE || current->prev->type == D_QUOTE)
				if (current->str[0] != ' ' && current->str[0] != '\t')
					fix_quote_before(shell, current);
		if (current->next)
			if (current->next->type == S_QUOTE || current->next->type == D_QUOTE)
				if (current->str[ft_strlen(current->str)] != ' '
				&& current->str[ft_strlen(current->str)] != '\t')
					fix_quote_after(shell, current);
		current = current->next;
	}
}
