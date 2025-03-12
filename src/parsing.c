/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 13:22:37 by meel-war          #+#    #+#             */
/*   Updated: 2025/03/12 16:44:04 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	new_node_token(t_token *ptr)
{
	t_token	*new;

	if (!ptr)
		return ;
	new = malloc(sizeof(t_token));
	if (!new)
		return ;
	while (ptr->next)
		ptr = ptr->next;
	ptr->next = new;
	new->prev = ptr;
	new->next = NULL;
}

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

static int	count_delim(char *line, int *k)
{
	int	*i;
	int	j;

	j = 0;
	i = k;
	while (line[*i] && (line[*i] == '|' || line[*i] == '>' || line[*i] == '<'))
	{
		j++;
		*i = *i + 1;
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

void	ft_minisplit(char *line, t_token *token)
{
	int		k;
	bool	delim;

	delim = false;
	k = 0;
	while (line[k])
	{
		while (line[k] && (line[k] == ' '
				|| line[k] == '\t' || line[k] == '\n'))
			k++;
		while (line[k] == '|' || line[k] == '>' || line[k] == '<')
		{
			token->str = fill_delim(line, &k);
			delim = true;
		}
		if (delim == true)
		{
			new_node_token(token);
			token = token->next;
		}
		delim = false;
		if (line[k] != '|' && line[k] != '>' && line[k] != '<')
			token->str = fill_word(line, &k);
		new_node_token(token);
		token = token->next;
	}
	token = NULL;
}

int main(void)
{
	char *line;
	t_token *tmp = malloc(sizeof(t_token));
	t_token *token = malloc(sizeof(t_token));
	line = malloc(sizeof(50000));
	line = "< file1 cat -e | ls -l >> file2";
	tmp = token;
	ft_minisplit(line, token);
	token = tmp;
	while (token->next)
	{
		printf("%s\n", token->str);
		token = token->next;
	}
}