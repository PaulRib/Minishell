/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 16:34:47 by meel-war          #+#    #+#             */
/*   Updated: 2025/05/05 15:11:28 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*get_var_name(char *str, int *i)
{
	int		start;
	int		len;
	char	*var_name;

	start = *i;
	len = 0;
	if (str[*i] == '?')
	{
		(*i)++;
		var_name = ft_strdup("?");
		return (var_name);
	}
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
	{
		(*i)++;
		len++;
	}
	if (len == 0)
		return (NULL);
	var_name = ft_substr(str, start, len);
	return (var_name);
}

static char	*get_env_or_exit_status(t_shell *shell, char *name)
{
	char	*result;
	char	*tmp;

	if (ft_strcmp(name, "?") == 0)
	{
		tmp = ft_itoa(shell->exit_status);
		// return (ft_itoa(g_exit_status));
		result = ft_strdup(tmp);
		free(tmp);
	}
	else
	{
		tmp = ft_get_env(shell->data->new_env, name);
		if (!tmp)
			result = ft_strdup("");
		else
			result = ft_strdup(tmp);
	}
	return (result);
}

static char	*handle_dollar(t_shell *shell, char *str, int *i)
{
	char	*name;
	char	*value;

	(*i)++;
	if (!str[*i] || str[*i] == ' ' || str[*i] == '\t' || str[*i] == '\n')
		return (ft_strdup("$"));
	name = get_var_name(str, i);
	if (!name)
		return (ft_strdup("$"));
	value = get_env_or_exit_status(shell, name);
	free(name);
	return (value);
}

char	*expand_variables(t_shell *shell, char *str)
{
	int		i;
	char	*result;
	char	*var;

	if (!str)
		return (NULL);
	i = 0;
	result = ft_strdup("");
	while (str[i])
	{
		if (str[i] == '$')
		{
			var = handle_dollar(shell, str, &i);
			update_result(&result, var);
			free(var);
		}
		else
			append_normal_char(&result, str, &i);
	}
	return (result);
}

void	expand_all_tokens(t_shell *shell)
{
	t_token	*cur;
	char	*expanded;

	cur = shell->token;
	while (cur)
	{
		if (cur->type == WORD || cur->type == D_QUOTE)
		{
			expanded = expand_variables(shell, cur->str);
			free(cur->str);
			cur->str = expanded;
		}
		cur = cur->next;
	}
}
