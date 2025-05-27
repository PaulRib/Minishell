/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 14:38:45 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/26 17:59:29 by meel-war         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	syntax_pipe(t_token *current)
{
	if (current->type == PIPE && (!current->prev || !current->next))
	{
		ft_putstr_fd("bash: syntax error near ", 2);
		ft_putstr_fd("unexpected token '|'\n", 2);
		return (-1);
	}
	if (current->type == PIPE && current->next->type != WORD
		&& current->prev->type != WORD && current->next->type != CMD
		&& current->prev->type != CMD && current->next->type != REDIR_IN)
	{
		ft_putstr_fd("bash: syntax error near ", 2);
		ft_putstr_fd("unexpected token '|'\n", 2);
		return (-1);
	}
	return (0);
}

static int	syntax_simple_redir(t_token *current)
{
	if (current->type == REDIR_IN && !current->next)
	{
		ft_putstr_fd("bash: syntax error near unexpected token newline\n", 2);
		return (-1);
	}
	if (current->type == REDIR_IN && current->next->type != WORD
		&& current->next->type != FILE_IN)
	{
		ft_putstr_fd("bash: syntax error near unexpected token '<'\n", 2);
		return (-1);
	}
	if (current->type == REDIR_OUT && !current->next)
	{
		ft_putstr_fd("bash: syntax error near unexpected token newline\n", 2);
		return (-1);
	}
	if (current->type == REDIR_OUT && current->next->type != WORD
		&& current->next->type != FILE_OUT)
	{
		ft_putstr_fd("bash: syntax error near unexpected token '>'\n", 2);
		return (-1);
	}
	return (0);
}

static int	syntax_double_redir(t_token *current)
{
	if (current->type == HEREDOC && !current->next)
	{
		ft_putstr_fd("bash: syntax error near unexpected token newline\n", 2);
		return (-1);
	}
	if (current->type == HEREDOC && current->next->type != WORD
		&& current->next->type != END)
	{
		ft_putstr_fd("bash: syntax error near unexpected token '<<'\n", 2);
		return (-1);
	}
	if (current->type == APPEND && !current->next)
	{
		ft_putstr_fd("bash: syntax error near unexpected token newline\n", 2);
		return (-1);
	}
	if (current->type == APPEND && current->next->type != WORD
		&& current->next->type != FILE_OUT)
	{
		ft_putstr_fd("bash: syntax error near unexpected token '>>'\n", 2);
		return (-1);
	}
	return (0);
}

int	syntax_hub(t_shell *shell)
{
	t_token	*current;

	current = shell->token;
	while (current->next)
	{
		if (syntax_pipe(current) == -1)
			return (-1);
		if (syntax_double_redir(current) == -1)
			return (-1);
		if (syntax_simple_redir(current) == -1)
			return (-1);
		current = current->next;
	}
	return (0);
}
