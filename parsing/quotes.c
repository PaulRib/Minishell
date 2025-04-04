/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 13:19:31 by pribolzi          #+#    #+#             */
/*   Updated: 2025/04/04 14:21:40 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_closed(t_token *current, int i, char c)
{
	while (current->str[i])
	{
		if (current->str[i] == c)
			return (i);
		i++;
	}
	return (0);
}

static void	after_quote(t_token *current, t_token *new, int end)
{
	t_token	*after;

	after = malloc(sizeof(t_token));
	after->str = ft_strdup(&current->str[end]);
	if (after->str)
	{
		after->next = new->next;
		if (new->next)
			new->next->prev = after;
		after->prev = new;
		new->next = after;
		after->type = WORD;
	}
	else
	{
		free(after->str);
		free(after);
	}
}

static void	extract_quote(t_token *current, int start, int end, char c)
{
	t_token	*new_token;
	char	*rest;

	new_token = malloc(sizeof(t_token));
	new_token->str = ft_substr(current->str, start, end - start);
	if (new_token->str[0] == '\0' || !new_token->str)
	{
		free(new_token->str);
		free(new_token);
		return ;
	}
	else
	{
		new_token->next = current->next;
		if (current->next)
			current->next->prev = new_token;
		new_token->prev = current;
		current->next = new_token;
		if (c == '"')
			new_token->type = D_QUOTE;
		else if (c == '\'')
			new_token->type = S_QUOTE;
	}
	rest = ft_substr(current->str, 0, start - 1);
	if (rest)
	{
		if (current->str[end + 1])
			after_quote(current, new_token, end + 1);
		free(current->str);
		current->str = rest;
	}
	else if (rest[0] == '\0' || !rest)
	{
		free(new_token->str);
		free(new_token);
	}
}

static void	process_quote(t_token *current)
{
	int	start;
	int	end;
	int	i;

	i = 0;
	while (current->str[i])
	{
		if (current->str[i] == '"')
		{
			end = is_closed(current, i + 1, '"');
			if (end != 0)
			{
				start = i + 1;
				extract_quote(current, start, end, '"');
				i = end;
			}
			else if (end == 0)
				ft_putstr_fd("Quotes are not closed\n", 2);
		}
		else if (current->str[i] == '\'')
		{
			end = is_closed(current, i + 1, '\'');
			if (end != 0)
			{
				start = i + 1;
				extract_quote(current, start, end, '\'');
				i = end;
			}
			else if (end == 0)
				ft_putstr_fd("Quotes are not closed\n", 2);
		}
		i++;
	}
}

void	handling_quotes(t_shell *shell)
{
	t_token	*tmp;

	tmp = shell->token;
	while (tmp->next)
	{
		if (tmp->type == WORD)
			process_quote(tmp);
		tmp = tmp->next;
	}
}
