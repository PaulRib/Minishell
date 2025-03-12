/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 15:14:24 by pribolzi          #+#    #+#             */
/*   Updated: 2025/03/12 18:23:08 by meel-war         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	get_type(t_token *token)
{
	int	i;

	i = 0;
	while (token->next)
	{
		if (ft_strcmp(token->str, "|") == 0)
			token->type = PIPE;
		else if (ft_strcmp(token->str, ">") == 0)
			token->type = REDIR_IN;
		else if (ft_strcmp(token->str, "<") == 0)
			token->type = REDIR_OUT;
		else if (ft_strcmp(token->str, ">>") == 0)
			token->type = APPEND;
		else if (ft_strcmp(token->str, "<<") == 0)
			token->type = HEREDOC;
		else
			token->type = WORD;
	}
}

// void osef(t_token *token)
// {
// 	int i;

// 	i = 0;
// 	while (token->str[i])
// 	{
// 		if (token->type[i] == REDIR_OUT)
// 			token->type[i + 1] == FD_OUT
// 		if (token->type[i + 1] == FD_OUT && token->type[i + 2] != PIPE)
// 			token->type[i + 2] == CMD
// 		if (token->type[i] == REDIR_IN)
// 		{
// 			token->type[i + 1] == FD_IN
// 			token->type[i - 1] == CMD
// 		}
// 	}
// }

void check_other_word(t_token *token)
{
	while(token->next)
	{
		if (token->type == WORD)
		{
			ft_split_word(token);
		}
	}
}

char *ft_split_word(t_token *token)
{
	int i;

	i = 0;
	while(token->str[i])
	{
		while (token->str[i] && token->str[i] == ' ' || token->str[i] == '\t' || token->str[i] == '\n')
			i++;
		while (token->str[i] && token->str[i] != ' ' && token->str[i] != '\t' && token->str[i] != '\n')
			i++;
		while (token->str[i] && token->str[i] == ' ' || token->str[i] == '\t' || token->str[i] == '\n')
			i++;
		if(token->str[i])
		{
			new_node_token(token, false);
			ft_strdup_token(token, i);
		}
	}
}

char	*ft_strdup_token(t_token *token, int j)
{
	char		*dest;
	int		i;

	if (!token->str)
		return (NULL);
	i = 0;
	dest = malloc(sizeof(char) * ft_strlen(token->str, j) + 1);
	if (!dest)
		return (NULL);
	while (token->str[j] && token->str[j] != ' ' && token->str[j] != '\t')
	{
		dest[i] = token->str[j];
		i++;
		j++;
	}
	dest[i] = '\0';
	return (dest);
}

int	ft_strlen(t_token *token, int j)
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

