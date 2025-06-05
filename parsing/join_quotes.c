/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_quotes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 11:44:29 by pribolzi          #+#    #+#             */
/*   Updated: 2025/06/05 17:02:11 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	*fix_quote_before(t_shell *shell, t_token *current)
{
	char	*final;
	int		end;
	char	*tmp;
	t_token	*prev;

	end = ft_strlen(current->str);
	tmp = ft_substr(current->str, 0, end);
	if (!tmp)
		free_all(shell, 1);
	final = ft_strjoin(current->prev->str, tmp);
	free(tmp);
	if (!final)
		free_all(shell, 1);
	free(current->prev->str);
	current->prev->str = final;
	prev = current->prev;
	prev->last_space = true;
	if (current->next)
		current->next->prev = prev;
	prev->next = current->next;
	if (shell->token == current)
		shell->token = prev;
	free(current->str);
	free(current);
	return (prev);
}

static t_token	*fix_quote_after(t_shell *shell, t_token *current)
{
	char	*tmp;
	char	*final;
	int		len;
	t_token	*next;

	len = ft_strlen(current->str);
	tmp = ft_substr(current->str, 0, len);
	if (!tmp)
		free_all(shell, 1);
	final = ft_strjoin(tmp, current->next->str);
	free(tmp);
	if (!final)
		free_all(shell, 1);
	free(current->next->str);
	current->next->str = final;
	next = current->next;
	if (current->prev)
		current->prev->next = next;
	next->prev = current->prev;
	if (shell->token == current)
	shell->token = next;
	free(current->str);
	free(current);
	return (next);
}

static int	should_join_quotes(t_token *current, int check_before)
{
	int	len;

	if (check_before)
	{
		if (current->prev && current->prev->last_space == true)
			return (0);
		if (current->prev && (current->prev->type == S_QUOTE
				|| current->prev->type == D_QUOTE))
			return (join_check(current, 0));
	}
	else
	{
		if (current->last_space == true)
			return (0);
		if (current->next && (current->next->type == S_QUOTE
				|| current->next->type == D_QUOTE))
		{
			len = ft_strlen(current->str);
			return (join_check(current, len));
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
		}
		if (should_join_quotes(current, 0))
		{
			current = fix_quote_after(shell, current);
			if (!current)
				break ;
		}
		current = current->next;
	}
}
