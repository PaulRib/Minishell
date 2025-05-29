/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_quotes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 11:44:29 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/29 23:14:14 by meel-war         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static t_token	*fix_quote_before(t_shell *shell, t_token *current)
{
	char	*final;
	int		end;
	char	*tmp;

	if (!current || !current->prev || !current->str)
		return (current);
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
	return (change_after(shell, current, end));
}

static t_token	*update_current_token(t_shell *shell,
		t_token *current, int start)
{
	char	*tmp;

	if (start >= 0)
	{
		tmp = ft_substr(current->str, 0, start + 1);
		if (!tmp)
			free_all(shell, 1);
		free(current->str);
		current->str = tmp;
		return (current->next);
	}
	else
	{
		if (current->prev)
			current->prev->next = current->next;
		else
			shell->token = current->next;
		if (current->next)
			current->next->prev = current->prev;
		free(current->str);
		free(current);
		return (current->next);
	}
}

static t_token	*fix_quote_after(t_shell *shell, t_token *current)
{
	char	*tmp;
	char	*final;
	int		start;
	int		len;

	if (!current || !current->next || !current->str)
		return (current);
	len = ft_strlen(current->str);
	start = find_space(current, len - 1);
	tmp = ft_substr(current->str, start + 1, len - start - 1);
	if (!tmp)
		free_all(shell, 1);
	final = ft_strjoin(tmp, current->next->str);
	free(tmp);
	if (!final)
		free_all(shell, 1);
	free(current->next->str);
	current->next->str = final;
	return (update_current_token(shell, current, start));
}

static int	should_join_quotes(t_token *current, int check_before)
{
	int	len;

	if (check_before)
	{
		if (current->prev && (current->prev->type == S_QUOTE
				|| current->prev->type == D_QUOTE))
		{
			if (current->str && current->str[0] != ' '
				&& current->str[0] != '\t')
				return (1);
		}
	}
	else
	{
		if (current->next && (current->next->type == S_QUOTE
				|| current->next->type == D_QUOTE))
		{
			len = ft_strlen(current->str);
			if (len > 0 && current->str[len - 1] != ' '
				&& current->str[len - 1] != '\t')
				return (1);
		}
	}
	return (0);
}

void	join_quote(t_shell *shell)
{
	t_token	*current;

	if (!shell || !shell->token)
		return ;
	current = shell->token;
	while (current)
	{
		if (should_join_quotes(current, 1))
		{
			current = fix_quote_before(shell, current);
			if (!current)
				break ;
			continue ;
		}
		if (should_join_quotes(current, 0))
		{
			current = fix_quote_after(shell, current);
			if (!current)
				break ;
			continue ;
		}
		if (current)
			current = current->next;
	}
}
