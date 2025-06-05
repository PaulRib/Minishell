/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:59:45 by pribolzi          #+#    #+#             */
/*   Updated: 2025/06/05 16:40:46 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	create_and_insert_token(t_token *current, char *remainder,
		t_shell *shell, char *first_word)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
	{
		free(first_word);
		free(remainder);
		free_all(shell, 1);
	}
	new_token = ft_memset(new_token, 0, sizeof(t_token));
	new_token->str = remainder;
	new_token->type = WORD;
	new_token->last_space = current->last_space;
	current->last_space = false;
	new_token->next = current->next;
	if (current->next)
		current->next->prev = new_token;
	new_token->prev = current;
	current->next = new_token;
}

static void	handle_operator_after_word(t_token *current, int word_start,
		int word_end, t_shell *shell)
{
	char	*first_word;
	char	*rest_str;

	first_word = ft_substr(current->str, word_start, word_end - word_start);
	rest_str = ft_strdup(&current->str[word_end]);
	create_and_insert_token(current, rest_str, shell, first_word);
	free(current->str);
	current->str = first_word;
}

static void	handle_regular_split(t_token *current, int word_start, t_parse prs,
		t_shell *shell)
{
	char	*first_word;
	char	*rest_str;

	first_word = ft_substr(current->str, word_start, prs.w_end - word_start);
	if (!first_word)
		free_all(shell, 1);
	rest_str = ft_strdup(&current->str[prs.next_word]);
	if (!rest_str)
	{
		free(first_word);
		free_all(shell, 1);
	}
	create_and_insert_token(current, rest_str, shell, first_word);
	free(current->str);
	current->str = first_word;
}

static void	process_token(t_token *current, t_shell *shell)
{
	t_parse	prs;
	int		w_start;
	char	*first_word;

	w_start = 0;
	prs.w_end = find_word_limit(current->str, &w_start);
	prs.next_word = 0;
	if (current->str[prs.w_end] && is_delimiter(current->str[prs.w_end]))
		prs.next_word = check_for_more_words(current->str, prs.w_end);
	else if (current->str[prs.w_end])
		prs.next_word = prs.w_end;
	if (!prs.next_word)
	{
		first_word = ft_substr(current->str, w_start, prs.w_end - w_start);
		if (!first_word)
			free_all(shell, 1);
		free(current->str);
		current->str = first_word;
		return ;
	}
	prs.op_len = is_special_operator(current->str, prs.w_end);
	if (prs.op_len > 0 && w_start != prs.w_end)
		handle_operator_after_word(current, w_start, prs.w_end, shell);
	else
		handle_regular_split(current, w_start, prs, shell);
}

void	ft_split_word(t_shell *shell)
{
	t_token	*current;

	current = shell->token;
	while (current)
	{
		if ((current->type == CMD || current->type == WORD) && current->str)
			process_token(current, shell);
		current = current->next;
	}
}
