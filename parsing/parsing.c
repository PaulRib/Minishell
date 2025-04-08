/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 13:22:37 by meel-war          #+#    #+#             */
/*   Updated: 2025/04/08 18:17:27 by pribolzi         ###   ########.fr       */
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

static char	*fill_word(char *s, int *k)
{
	char	*word;
	int		len;
	int		i;

	len = ft_strlen_mod(&s[*k]);
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
	word = malloc(sizeof(count + 1));
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
	t_token	*tmp;

	tmp = shell->token;
	k = 0;
	while (line[k])
	{
		while (line[k] && (line[k] == ' ' || line[k] == '\t'
				|| line[k] == '\n'))
			k++;
		if (!line[k])
			break ;
		if (line[k] == '|' || line[k] == '>' || line[k] == '<')
		{
			tmp->str = fill_delim(line, &k);
			new_node_token(tmp);
			tmp = tmp->next;
		}
		else if (line[k])
		{
			tmp->str = fill_word(line, &k);
			new_node_token(tmp);
			tmp = tmp->next;
		}
	}
}
