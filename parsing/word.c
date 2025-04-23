/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:59:45 by pribolzi          #+#    #+#             */
/*   Updated: 2025/04/23 17:47:14 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	create_and_insert_token(t_token *current, char *remainder)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return ;
	new_token = ft_memset(new_token, 0, sizeof(t_token));
	new_token->str = remainder;
	new_token->type = WORD;
	new_token->next = current->next;
	if (current->next)
		current->next->prev = new_token;
	new_token->prev = current;
	current->next = new_token;
}

static int	find_word_boundaries(char *str, int *start)
{
	int	i;

	i = *start;
	while (str[i] && (str[i] == ' ' || str[i] == '\t' || str[i] == '\n'))
		i++;
	*start = i;
	while (str[i] && str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
		i++;
	return (i);
}

int	check_for_more_words(char *str, int i)
{
	while (str[i] && (str[i] == ' ' || str[i] == '\t' || str[i] == '\n'))
		i++;
	if (!str[i])
		return (0);
	return (i);
}

static void	process_token(t_token *current)
{
	int		word_start;
	int		word_end;
	int		next_word;
	char	*first_word;
	char	*rest_str;

	word_start = 0;
	word_end = find_word_boundaries(current->str, &word_start);
	next_word = check_for_more_words(current->str, word_end);
	if (next_word)
	{
		first_word = ft_substr(current->str, word_start, word_end - word_start);
		rest_str = ft_strdup(&current->str[next_word]);
		create_and_insert_token(current, rest_str);
		free(current->str);
		current->str = first_word;
	}
	else
	{
		first_word = ft_substr(current->str, 0, word_end);
		free(current->str);
		current->str = first_word;
	}
}

void	ft_split_word(t_shell *shell)
{
	t_token	*current;

	current = shell->token;
	while (current)
	{
		if (current->type == WORD && current->str)
			process_token(current);
		current = current->next;
	}
}
