/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 13:22:37 by meel-war          #+#    #+#             */
/*   Updated: 2025/03/04 15:11:50 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	ft_strlen_mod(char *s)
{
	int	i;

	i = 0;
	while (*s && *s != '|' && *s != '>' && *s != '<')
	{
		s++;
		i++;
	}
	return (i);
}

static size_t	count_words(char *s)
{
	size_t	count;
	size_t	i;

	count = 0;
	i = 0;
	if (s[i] == '\0')
		return (0);
	while (s[i])
	{
		if (s[i] && (s[i] != '|' && s[i] != '>' && s[i] != '<')
			&& (i == 0 || s[i] == '>' || s[i] == '<'))
			count++;
		if (s[i] == '|' || s[i] == '>' || s[i] == '<')
			count++;
		i++;
	}
	return (count);
}

static char	*fill_word(char *s, int *k)
{
	char	*word;
	int		len;
	int		i;

	len = ft_strlen_mod(s);
	word = malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	i = 0;
	while (s[*k] && s[*k] != '|' && s[*k] != '>' && s[*k] != '<')
	{
		word[i] = s[*k];
		*k = *k + 1;
		i++;
	}
	word[i] = '\0';
	return (word);
}

void	ft_minisplit(char *line, t_token *token)
{
	int	k;
	int	i;

	i = 0;
	token->str = malloc(sizeof(char *) * (count_words(line) + 1));
	if (!token->str)
		return (free_tab(token->str));
	k = 0;
	while (line[k])
	{
		while (line[k] && (line[k] == ' '
				|| line[k] == '\t' || line[k] == '\n'))
			k++;
		if (line[k] != '|' && line[k] != '>' && line[k] != '<')
			token->str[i] = fill_word(line, &k);
		i++;
		if (line[k] == '|' || line[k] == '>' || line[k] == '<')
		{
			token->str[i] = malloc(sizeof(char) * (2));
			token->str[i][0] = line[k++];
			i++;
		}
	}
	token->str[i] = NULL;
}
