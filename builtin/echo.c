/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 14:15:41 by meel-war          #+#    #+#             */
/*   Updated: 2025/06/02 11:58:17 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	new_node_echo(t_token *current, int start, t_shell *shell)
{
	t_token	*new;
	char	*echo;

	new = malloc(sizeof(t_token));
	if (!new)
		free_all(shell, 1);
	new->str = ft_substr(current->str, start, ft_strlen(current->str) - start);
	if (!new->str)
	{
		free(new);
		free_all(shell, 1);
	}
	free(current->str);
	new->type = WORD;
	new->next = current->next;
	current->next = new;
	current->next->prev = new;
	new->prev = current;
	echo = ft_substr(current->str, 0, start - 1);
	if (!echo)
		free_all(shell, 1);
	current->str = echo;
}

static int	is_n_flag(char *str, int i, int *end)
{
	if (str[i] == '-' && str[i + 1] == 'n')
	{
		i += 2;
		while (str[i] == 'n')
			i++;
		if (str[i] == '\0' || str[i] == ' ')
		{
			if (str[i] == ' ')
				i++;
			*end = i;
			return (1);
		}
	}
	return (0);
}

int	ft_count(t_token *current, t_shell *shell)
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
		if (is_n_flag(tmp->str, i, &i))
			n++;
		else if (tmp->str[i] != '\0' && tmp->str[i] != ' ')
		{
			new_node_echo(tmp, i, shell);
			return (n);
		}
		else
			i++;
	}
	return (n);
}

static int	ft_echo(t_token *token_ptr, t_shell *shell)
{
	t_token	*current;
	int		n_param;

	n_param = 0;
	n_param = ft_count(token_ptr, shell);
	current = token_ptr->next;
	while (current && current->type != PIPE)
	{
		if (current->type == WORD || current->type == S_QUOTE
			|| current->type == D_QUOTE)
		{
			ft_putstr_fd(current->str, 1);
			if (current->next)
				write(1, " ", 1);
		}
		current = current->next;
	}
	if (n_param == 0)
		write(1, "\n", 1);
	return (0);
}

int	check_echo(t_token *token_ptr, t_shell *shell)
{
	if (ft_strncmp(token_ptr->str, "echo ", 5) != 0
		&& ft_strncmp(token_ptr->str, "echo", 5) != 0)
		return (-1);
	return (ft_echo(token_ptr, shell));
}
