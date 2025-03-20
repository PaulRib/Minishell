/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 15:14:24 by pribolzi          #+#    #+#             */
/*   Updated: 2025/03/20 14:44:03 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	get_type(t_shell *shell)
{
	int			i;
	t_token		*tmp;

	tmp = shell->token;
	i = 0;
	while (tmp != NULL)
	{
		if (ft_strcmp(tmp->str, "|") == 0)
			tmp->type = PIPE;
		else if (ft_strcmp(tmp->str, ">") == 0)
			tmp->type = REDIR_OUT;
		else if (ft_strcmp(tmp->str, "<") == 0)
			tmp->type = REDIR_IN;
		else if (ft_strcmp(tmp->str, ">>") == 0)
			tmp->type = APPEND;
		else if (ft_strcmp(tmp->str, "<<") == 0)
			tmp->type = HEREDOC;
		else
			tmp->type = WORD;
		tmp = tmp->next;
	}
}

// static void	check_other_word(t_token *token)
// {
// 	while (token != NULL)
// 	{
// 		if (token->type == WORD)
// 		{
// 			ft_split_word(token);
// 		}
// 		token = token->next;
// 	}
// }

static int	ft_strlen_token(t_token *token, int j)
{
	int	i;

	if (!token->str)
		return (-1);
	i = 0;
	while (token->str[j] && token->str[j] != ' ' && token->str[j] != '\t')
	{
		j++;
		i++;
	}
	return (i);
}

static void	ft_remove_token_word(t_token *token, int j)
{
	int		word_len;
	int		i;
	int		k;
	int		total_len;
	char	*new_str;

	total_len = ft_strlen(token->str);
	k = -1;
	i = j;
	word_len = 0;
	while (token->str[i] && token->str[i] != ' ' && token->str[i++] != '\t')
		word_len++;
	while (token->str[i] == ' ' || token->str[i] == '\t')
		i++;
	new_str = malloc(sizeof(char) * total_len - word_len + 1);
	if (!new_str)
		return ;
	while (++k < j)
		new_str[k] = token->str[k];
	while (token->str[i])
		new_str[k++] = token->str[i++];
	new_str[k] = '\0';
	free(token->str);
	token->str = new_str;
}

void	ft_strdup_token(t_token *token, int j)
{
	char		*dest;
	int			i;
	int			start_j;
	t_token		*tmp;

	start_j = j;
	tmp = token;
	if (!token->str)
		return ;
	i = 0;
	dest = malloc(sizeof(char) * ft_strlen_token(token, j) + 1);
	if (!dest)
		return ;
	while (token->str[j] && token->str[j] != ' ' && token->str[j] != '\t')
	{
		dest[i] = token->str[j];
		i++;
		j++;
	}
	dest[i] = '\0';
	ft_remove_token_word(token, start_j);
	while (token->next)
		token = token->next;
	token->str = dest;
	token = tmp;
}

void	ft_split_word(t_shell *shell)
{
	int		i;
	t_token	*tmp;

	tmp = shell->token;
	i = 0;
	while (tmp->str[i])
	{
		while (tmp->str[i] && (tmp->str[i] == ' '
				|| tmp->str[i] == '\t' || tmp->str[i] == '\n'))
			i++;
		while (tmp->str[i] && (tmp->str[i] != ' '
				&& tmp->str[i] != '\t' && tmp->str[i] != '\n'))
			i++;
		while (tmp->str[i] && (tmp->str[i] == ' '
				|| tmp->str[i] == '\t' || tmp->str[i] == '\n'))
			i++;
		if (tmp->str[i])
		{
			new_node_token(tmp, true, NULL);
			ft_strdup_token(tmp, i);
		}
		break ;
	}
}
