/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:56:31 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/21 15:27:44 by pribolzi         ###   ########.fr       */
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

void	empty_quote_before(t_token *current, t_token *new, t_quote qte,
		t_shell *shell)
{
	if (qte.c == '"')
		current->type = D_QUOTE;
	else if (qte.c == '\'')
		current->type = S_QUOTE;
	if (current->str[qte.end + 1])
		after_mult_quote(current, qte.end + 1, shell);
	free(current->str);
	current->str = new->str;
	free(new);
}

void	before_quote(t_token *current, int start, t_shell *shell)
{
	char	*rest;

	while (start > 0 && (current->str[start] == ' '
			|| current->str[start] == '\t' || current->str[start] == '\n'))
		start--;
	if (start > 0)
	{
		rest = ft_substr(current->str, 0, start - 1);
		if (!rest)
			free_all(shell);
		free(current->str);
		current->str = rest;
	}
}

void	after_mult_quote(t_token *current, int end, t_shell *shell)
{
	t_token	*after;

	after = malloc(sizeof(t_token));
	if (!after)
		free_all(shell);
	after->str = ft_strdup(&current->str[end]);
	if (!after->str)
	{
		free(after);
		free_all(shell);
	}
	if (after->str && after->str[0] != '\0' && is_empty(after, 2147483647))
	{
		after->next = current->next;
		after->prev = current;
		current->next = after;
		after->type = WORD;
	}
	else
	{
		if (after->str)
			free(after->str);
		if (after)
			free(after);
	}
}
