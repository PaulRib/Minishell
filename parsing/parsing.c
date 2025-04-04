/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 13:22:37 by meel-war          #+#    #+#             */
/*   Updated: 2025/04/04 16:34:36 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_in_quotes(char *s, int pos)
{
	int		i;
	char	quote_char;
	int		in_quote;

	i = 0;
	in_quote = 0;
	quote_char = 0;
	while (i < pos)
	{
		if ((s[i] == '\'' || s[i] == '\"') && !in_quote)
		{
			in_quote = 1;
			quote_char = s[i];
		}
		else if (in_quote && s[i] == quote_char)
			in_quote = 0;
		i++;
	}
	return (in_quote);
}

static int	ft_strlen_mod(char *s)
{
	int	i;
	int	in_quote;

	i = 0;
	while (s[i])
	{
		in_quote = is_in_quotes(s, i);
		if ((s[i] == '|' || s[i] == '>' || s[i] == '<') && !in_quote)
			break ;
		i++;
	}
	return (i);
}

static char	*fill_word(char *s, int *k)
{
	char	*word;
	int		len;
	int		i;
	int		in_quote;

	len = ft_strlen_mod(&s[*k]);
	word = malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	i = 0;
	while (s[*k])
	{
		in_quote = is_in_quotes(s, *k);
		if ((s[*k] == '|' || s[*k] == '>' || s[*k] == '<') && !in_quote)
			break ;
		word[i] = s[*k];
		*k = *k + 1;
		i++;
	}
	word[i] = '\0';
	return (word);
}

static int	count_delim(char *line, int *k)
{
	int	i;
	int	j;

	j = 0;
	i = *k;
	while (line[i] && (line[i] == '|' || line[i] == '>' || line[i] == '<'))
	{
		j++;
		i = i + 1;
	}
	return (j);
}

static char	*fill_delim(char *s, int *k)
{
	char	*word;
	int		i;
	int		count;

	count = count_delim(s, k);
	word = malloc(sizeof(char) * (count + 1));
	if (!word)
		return (NULL);
	i = 0;
	while (s[*k] && (s[*k] == '|' || s[*k] == '>' || s[*k] == '<'))
	{
		word[i] = s[*k];
		*k = *k + 1;
		i++;
	}
	word[i] = '\0';
	return (word);
}

void	ft_minisplit(char *line, t_shell *shell)
{
	int		k;
	bool	delim;
	t_token	*tmp;
	int		in_quote;

	tmp = shell->token;
	delim = false;
	k = 0;
	while (line[k])
	{
		while (line[k] && (line[k] == ' '
				|| line[k] == '\t' || line[k] == '\n'))
			k++;
		if (!line[k])
			break ;
			
		in_quote = is_in_quotes(line, k);
		if ((line[k] == '|' || line[k] == '>' || line[k] == '<') && !in_quote)
		{
			tmp->str = fill_delim(line, &k);
			delim = true;
		}
		else
		{
			tmp->str = fill_word(line, &k);
			delim = true;
		}
		
		if (delim == true)
		{
			new_node_token(tmp);
			tmp = tmp->next;
		}
		delim = false;
	}
	tmp = NULL;
}