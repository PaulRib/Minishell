/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 14:15:41 by meel-war          #+#    #+#             */
/*   Updated: 2025/05/10 18:24:35 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	new_node_echo(t_token *current, int start)
{
	t_token	*new_token;
	char	*echo_str;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return ;
	new_token->str = ft_substr(current->str, start, 
			ft_strlen(current->str) - start);
	echo_str = ft_substr(current->str, 0, start);
	free(current->str);
	current->str = echo_str;
	new_token->type = WORD;
	new_token->next = current->next;
	if (current->next)
		current->next->prev = new_token;
	new_token->prev = current;
	current->next = new_token;
}

static int	handle_n_option_in_str(const char *str, int *i, int *n_opt_count)
{
	if (ft_strncmp(&str[*i], "-n", 2) == 0)
	{
		*i += 2;
		(*n_opt_count)++;
		while (str[*i] == 'n')
			(*i)++;
		return (1);
	}
	return (0);
}

static int	process_options_in_token(t_token *echo_cmd_token)
{
	int	i;
	int	n_options_count;

	i = 4;
	n_options_count = 0;
	while (echo_cmd_token->str && echo_cmd_token->str[i])
	{
		while (echo_cmd_token->str[i] == ' ')
			i++;
		if (echo_cmd_token->str[i] == '\0')
			break ;
		if (!handle_n_option_in_str(echo_cmd_token->str, &i, &n_options_count))
		{
			if (echo_cmd_token->str[i] != '\0')
				new_node_echo(echo_cmd_token, i);
			break;
		}
	}
	return (n_options_count);
}

static int	is_valid_n_option_token(const char *s)
{
	int	j;

	if (!s || s[0] != '-' || s[1] != 'n')
		return (0);
	j = 2;
	while (s[j] == 'n')
		j++;
	if (s[j] == '\0')
		return (1);
	return (0);
}

static int	check_and_count_n_options(t_token *token_ptr, t_token **first_arg)
{
	int	n_opt;

	n_opt = process_options_in_token(token_ptr);
	*first_arg = token_ptr->next;
	while (*first_arg && (*first_arg)->str && 
			is_valid_n_option_token((*first_arg)->str) &&
			((*first_arg)->type == WORD || (*first_arg)->type == S_QUOTE || 
			(*first_arg)->type == D_QUOTE))
	{
		n_opt++;
		*first_arg = (*first_arg)->next;
	}
	return (n_opt);
}

static void	print_special_case(t_token *arg1, t_token *arg2, t_token *arg3)
{
	ft_putstr_fd(arg1->str, 1);
	ft_putstr_fd(arg2->str, 1);
	ft_putstr_fd(arg3->str, 1);
}

static int	is_matching_quote_arg(const char *str, char quote_char)
{
	if (str && str[0] == quote_char && str[1] == '\0')
		return (1);
	return (0);
}

static int	is_special_echo_case(t_token *current_arg)
{
	char	first_quote_char;

	if (!current_arg || !current_arg->next || !current_arg->next->next)
		return (0);
	if (is_matching_quote_arg(current_arg->str, '"'))
		first_quote_char = '"';
	else if (is_matching_quote_arg(current_arg->str, '\''))
		first_quote_char = '\'';
	else
		return (0);
	if (is_matching_quote_arg(current_arg->next->next->str, first_quote_char))
		return (1);
	return (0);
}

static void	print_normal_echo_args(t_token *current_arg)
{
	int	first_arg_printed;

	first_arg_printed = 0;
	while (current_arg && (current_arg->type == WORD ||
			current_arg->type == S_QUOTE || current_arg->type == D_QUOTE))
	{
		if (first_arg_printed)
			write(1, " ", 1);
		ft_putstr_fd(current_arg->str, 1);
		first_arg_printed = 1;
		current_arg = current_arg->next;
	}
}

int	ft_echo(t_token *token_ptr)
{
	t_token	*current_arg;
	int		n_option_present;

	n_option_present = check_and_count_n_options(token_ptr, &current_arg);
	if (is_special_echo_case(current_arg))
	{
		print_special_case(current_arg, current_arg->next, 
			current_arg->next->next);
	}
	else
	{
		print_normal_echo_args(current_arg);
	}
	if (n_option_present == 0)
		write(1, "\n", 1);
	return (0);
}

int	check_echo(t_token *token_ptr)
{
	if (!token_ptr || !token_ptr->str || 
		ft_strncmp(token_ptr->str, "echo", 4) != 0)
		return (-1);
	return (ft_echo(token_ptr));
}
