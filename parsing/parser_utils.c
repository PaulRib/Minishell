/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 18:14:44 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/27 14:43:08 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_for_more_words(char *str, int i)
{
	i = skip_whitespace(str, i);
	if (!str[i])
		return (0);
	return (i);
}

int	is_delimiter(char c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '|' || c == '<' || c == '>');
}

int	is_special_operator(char *str, int i)
{
	if (str[i] == '|' || str[i] == '<' || str[i] == '>')
	{
		if ((str[i] == '<' && str[i + 1] == '<')
			|| (str[i] == '>' && str[i + 1] == '>'))
			return (2);
		return (1);
	}
	return (0);
}

int	skip_whitespace(char *str, int i)
{
	while (str[i] && (str[i] == ' ' || str[i] == '\t' || str[i] == '\n'))
		i++;
	return (i);
}

int	find_word_limit(char *str, int *start)
{
	int	i;
	int	op_len;

	i = skip_whitespace(str, *start);
	*start = i;
	op_len = is_special_operator(str, i);
	if (op_len > 0)
		return (i + op_len);
	while (str[i] && !is_delimiter(str[i]))
		i++;
	return (i);
}
