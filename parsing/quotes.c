/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 13:19:31 by pribolzi          #+#    #+#             */
/*   Updated: 2025/06/05 17:18:04 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	after_quote(t_token *current, t_token *new, int end, t_shell *shell)
{
	t_token	*after;

	after = malloc(sizeof(t_token));
	if (!after)
		free_all(shell, 1);
	ft_memset(after, 0, sizeof(t_token));
	after->str = ft_strdup(&current->str[end]);
	if (!after->str)
	{
		free(after);
		free_all(shell, 1);
	}
	if (after->str && after->str[0] != '\0' && is_empty(after, 2147483647))
	{
		after->next = new->next;
		if (new->next)
			new->next->prev = after;
		after->prev = new;
		new->next = after;
	}
	else
	{
		free(after->str);
		free(after);
	}
}

static void	extract_quote(t_token *current, t_quote qte, t_shell *shell)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		free_all(shell, 1);
	new_token->str = ft_substr(current->str, qte.start, qte.end - qte.start);
	new_token->last_space = false;
	if (!new_token->str)
	{
		free(new_token);
		free_all(shell, 1);
	}
	if (current->str[qte.end + 1] == ' ' || current->str[qte.end + 1] == '\t')
		new_token->last_space = true;
	if (is_empty(current, qte.start - 1))
	{
		insert_new_token(qte, new_token, current);
		if (current->str[qte.end + 1])
			after_quote(current, new_token, qte.end + 1, shell);
		before_quote(current, qte.start - 1, shell);
	}
	else
		empty_quote_before(current, new_token, qte, shell);
}

static int	process_quote2(t_token *current, int i, char c, t_shell *shell)
{
	t_quote	quote;

	quote.c = c;
	quote.end = is_closed(current, i + 1, quote.c);
	if (quote.end != 0)
	{
		quote.start = i + 1;
		extract_quote(current, quote, shell);
		i = quote.end;
	}
	else if (quote.end == 0)
	{
		ft_putstr_fd("Quotes are not closed\n", 2);
		return (-1);
	}
	return (i);
}

static int	process_quote(t_token *current, t_shell *shell)
{
	int	i;

	i = 0;
	while (current->str[i])
	{
		if (current->str[i] == '"')
		{
			i = process_quote2(current, i, '"', shell);
			return (i);
		}
		else if (current->str[i] == '\'')
		{
			i = process_quote2(current, i, '\'', shell);
			return (i);
		}
		i++;
	}
	return (1);
}

int	handling_quotes(t_shell *shell)
{
	t_token	*tmp;

	tmp = shell->token;
	while (tmp)
	{
		if (tmp->type == WORD || tmp->type == CMD)
			if (process_quote(tmp, shell) == -1)
				return (-1);
		tmp = tmp->next;
	}
	return (1);
}
