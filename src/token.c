/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 15:14:24 by pribolzi          #+#    #+#             */
/*   Updated: 2025/03/13 15:45:49 by meel-war         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	get_type(t_token *token)
{
	int	i;

	i = 0;
	while (token != NULL)
	{
		if (ft_strcmp(token->str, "|") == 0)
			token->type = PIPE;
		else if (ft_strcmp(token->str, ">") == 0)
			token->type = REDIR_OUT;
		else if (ft_strcmp(token->str, "<") == 0)
			token->type = REDIR_IN;
		else if (ft_strcmp(token->str, ">>") == 0)
			token->type = APPEND;
		else if (ft_strcmp(token->str, "<<") == 0)
			token->type = HEREDOC;
		else
			token->type = WORD;
		token = token->next;	
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

static void check_other_word(t_token *token)
{
	while(token != NULL)
	{
		if (token->type == WORD)
		{
			ft_split_word(token);
		}
		token = token->next;
	}
}

void ft_split_word(t_token *token)
{
	int i;

	i = 0;
	while(token->str[i])
	{
		while (token->str[i] && (token->str[i] == ' ' || token->str[i] == '\t' || token->str[i] == '\n'))
			i++;
		while (token->str[i] && (token->str[i] != ' ' && token->str[i] != '\t' && token->str[i] != '\n'))
			i++;
		while (token->str[i] && (token->str[i] == ' ' || token->str[i] == '\t' || token->str[i] == '\n'))
			i++;
		if(token->str[i])
		{
			new_node_token(token, false);
			ft_strdup_token(token, i);
		}
	}
}

 static char	*ft_strdup_token(t_token *token, int j)
{
	char		*dest;
	int		i;
	int start_j = j;

	if (!token->str)
		return (NULL);
	i = 0;
	dest = malloc(sizeof(char) * ft_strlen_token(token->str, j) + 1);
	if (!dest)
		return (NULL);
	while (token->str[j] && token->str[j] != ' ' && token->str[j] != '\t')
	{
		dest[i] = token->str[j];
		i++;
		j++;
	}
	dest[i] = '\0';
	ft_remove_token_word(token, start_j);
	return (dest);
}
static void ft_remove_token_word(t_token *token, int j)
{
	int word_len = 0;
	int i = j;
	int k = 0;
	int total_len = ft_strlen(token->str, 0);
	char *new_str;
	
		while(token->str[i] && (token->str[i] != ' ' || token->str[i] != '\t'))
		{
			word_len++;
			i++;
		}
		while(token->str[i] == ' ' || token->str[i] == '\t')
			i++;
		new_str = malloc(sizeof(char) * total_len - word_len + 1);
		if(!new_str)
			return(NULL);
		while(k < j)
			new_str[k++] = token->str[k++];
		while(token->str[i])
			new_str[k++] = token->str[i++];
		new_str[k] = '\0';
	free(token->str);
	token->str = new_str;
}

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

