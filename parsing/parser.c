/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:59:45 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/10 18:33:01 by pribolzi         ###   ########.fr       */
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

static void	handle_operator_after_word(t_token *current, int word_start,
										int word_end)
{
	char	*first_word;
	char	*rest_str;

	first_word = ft_substr(current->str, word_start, word_end - word_start);
	rest_str = ft_strdup(&current->str[word_end]);
	create_and_insert_token(current, rest_str);
	free(current->str);
	current->str = first_word;
}

static void	handle_regular_split(t_token *current, int word_start, int word_end,
								int next_word)
{
	char	*first_word;
	char	*rest_str;

	first_word = ft_substr(current->str, word_start, word_end - word_start);
	rest_str = ft_strdup(&current->str[next_word]);
	create_and_insert_token(current, rest_str);
	free(current->str);
	current->str = first_word;
}

static void	process_token(t_token *current)
{
	int		word_start;
	int		word_end;
	int		next_word;
	int		op_len;
	char	*first_word;

	word_start = 0;
	word_end = find_word_boundaries(current->str, &word_start);
	next_word = 0;
	if (current->str[word_end] && is_delimiter(current->str[word_end]))
		next_word = check_for_more_words(current->str, word_end);
	else if (current->str[word_end])
		next_word = word_end;
	if (!next_word)
	{
		first_word = ft_substr(current->str, word_start, word_end - word_start);
		free(current->str);
		current->str = first_word;
		return ;
	}
	op_len = is_special_operator(current->str, word_end);
	if (op_len > 0 && word_start != word_end)
		handle_operator_after_word(current, word_start, word_end);
	else
		handle_regular_split(current, word_start, word_end, next_word);
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
