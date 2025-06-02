/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 14:15:41 by meel-war          #+#    #+#             */
/*   Updated: 2025/06/02 13:01:19 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_n_flag(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' && str[i + 1] == 'n')
	{
		i += 2;
		while (str[i] == 'n')
			i++;
		if (str[i] == '\0')
			return (1);
	}
	return (0);
}

int	ft_count(t_token *current)
{
	int		n;

	n = 0;
	while (current && current->type != PIPE)
	{
		if (is_n_flag(current->str))
			n++;
		else
			return (n);
		current = current->next;
	}
	return (n);
}

static int	ft_echo(t_token *token_ptr)
{
	t_token	*curr;
	int		n_param;
	int		i;

	n_param = 0;
	n_param = ft_count(token_ptr->next);
	i = n_param;
	curr = token_ptr->next;
	while (curr && i--)
		curr = curr->next;
	while (curr && curr->type != PIPE)
	{
		if (curr->type == WORD || curr->type == S_QUOTE
			|| curr->type == D_QUOTE)
		{
			ft_putstr_fd(curr->str, 1);
			if (curr->next && curr->next->type != PIPE
				&& curr->next->type != REDIR_OUT && curr->next->type != REDIR_IN)
				write(1, " ", 1);
		}
		curr = curr->next;
	}
	if (n_param == 0)
		write(1, "\n", 1);
	return (0);
}

int	check_echo(t_token *token_ptr)
{
	if (ft_strncmp(token_ptr->str, "echo ", 5) != 0
		&& ft_strncmp(token_ptr->str, "echo", 5) != 0)
		return (-1);
	return (ft_echo(token_ptr));
}
