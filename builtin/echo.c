/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 14:15:41 by meel-war          #+#    #+#             */
/*   Updated: 2025/04/15 16:36:11 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	new_node_echo(t_token *current, int start)
{
	t_token	*new;
	char	*echo;

	new = malloc(sizeof(t_token));
	new->str = ft_substr(current->str, start, ft_strlen(current->str) - start);
	echo = ft_substr(current->str, 0, start - 1);
	free (current->str);
	new->type = WORD;
	current->str = echo;
	new->next = current->next;
	current->next->prev = new;
	new->prev = current;
	current->next = new;
}

int	ft_count(t_token *current)
{
	int		i;
	int		n;
	t_token	*tmp;

	tmp = current;
	i = 0;
	n = 0;
	while (tmp->str[i])
	{
		while (i < 4)
			i++;
		if (ft_strncmp(&tmp->str[i], "-n", 2) == 0)
		{
			i += 2;
			n++;
		}
		else if (tmp->str[i] != '\0' && tmp->str[i] != ' ')
		{
			new_node_echo(tmp, i);
			return (n);
		}
		i++;
	}
	return (n);
}

int	ft_echo(t_token *token_ptr)
{
	t_token	*current;
	int		n_param;

	n_param = 0;
	n_param = ft_count(token_ptr);
	current = token_ptr->next;
	while (current && current->type == WORD)
	{
		ft_putstr_fd(current->str, 1);
		if (current->next && current->next->type == WORD)
			write(1, " ", 1);
		current = current->next;
	}
	if (n_param == 0)
		write(1, "\n", 1);
	return (0);
}

int	check_echo(t_token *token_ptr)
{
	if (ft_strncmp(token_ptr->str, "echo", 4) != 0)
		return (-1);
	return (ft_echo(token_ptr));
}