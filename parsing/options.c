/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:13:24 by pribolzi          #+#    #+#             */
/*   Updated: 2025/04/04 15:58:08 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static bool	check_token_type(t_token *token, int check_type)
{
	int i;

	i = 0;
	if (!token || !token->str)
		return (false);
	if (token->type != CMD && token->type != WORD
		&& token->type != S_QUOTE && token->type != D_QUOTE)
		return (false);
	while (token->str[i] == ' ')
		i++;
	if (check_type == 1)
		return (token->str[i] == '-');
	else if (check_type == 2)
		return (token->str[i] != '-');
	return (false);
}

static char	*clean_string(const char *str)
{
	int		i;
	int		j;
	char	*result;

	result = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	while (str[i])
	{
		if (str[i] == ' ' || str[i] == '\t')
		{
			result[j++] = ' ';
			while (str[i] && (str[i] == ' ' || str[i] == '\t'))
				i++;
		}
		else
			result[j++] = str[i++];
	}
	if (j > 0 && result[j - 1] == ' ')
		j--;
	result[j] = '\0';
	return (result);
}

static bool	combine_tokens(t_token *command, t_token *option)
{
	char	*temp;
	char	*combined;
	char	*cleaned;

	temp = ft_strjoin(command->str, " ");
	if (!temp)
		return (false);
	combined = ft_strjoin(temp, option->str);
	free(temp);
	if (!combined)
		return (false);
	cleaned = clean_string(combined);
	free(combined);
	if (!cleaned)
		return (false);
	free(command->str);
	command->str = cleaned;
	return (true);
}

static t_token	*remove_token(t_token *option)
{
	t_token	*next_token;

	next_token = option->next;
	if (option->prev)
		option->prev->next = option->next;
	if (option->next)
		option->next->prev = option->prev;
	free(option->str);
	free(option);
	return (next_token);
}

void	associate_options_commands(t_shell *shell)
{
	t_token	*current;
	t_token	*next;
	t_token	*last_command;

	if (!shell || !shell->token)
		return ;
	current = shell->token;
	last_command = NULL;
	while (current)
	{
		next = current->next;
		if (current->type >= PIPE && current->type <= FILE_OUT)
			last_command = NULL;
		if (check_token_type(current, 2))
			last_command = current;
		else if (check_token_type(current, 1) && last_command)
		{
			if (combine_tokens(last_command, current))
				next = remove_token(current);
			else
				next = current->next;
		}
		current = next;
	}
}
