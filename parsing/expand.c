/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 16:34:47 by meel-war          #+#    #+#             */
/*   Updated: 2025/06/05 17:36:20 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*get_var_name(char *str, int *i, t_shell *shell)
{
	int		start;
	int		len;
	char	*var_name;

	start = *i;
	len = 0;
	if (str[*i] == '?')
	{
		(*i)++;
		var_name = safe_strdup("?", shell);
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
	if (!var_name)
		free_all(shell, 1);
	return (var_name);
}

static char	*get_env_or_exit_status(t_shell *shell, char *name)
{
	char	*result;
	char	*tmp;

	if (ft_strcmp(name, "?") == 0)
	{
		tmp = ft_itoa(shell->exit_status);
		return (tmp);
	}
	else
	{
		tmp = ft_get_env(shell->data->new_env, name);
		if (!tmp)
			result = safe_strdup("", shell);
		else
			result = safe_strdup(tmp, shell);
	}
	return (result);
}

char	*handle_dollar(t_shell *shell, char *str, int *i)
{
	char	*name;
	char	*value;

	(*i)++;
	if (!str[*i] || str[*i] == ' ' || str[*i] == '\t' || str[*i] == '\n')
		return (safe_strdup("$", shell));
	name = get_var_name(str, i, shell);
	if (!name)
		return (safe_strdup("$", shell));
	value = get_env_or_exit_status(shell, name);
	free(name);
	return (value);
}

char	*expand_variables(t_shell *shell, char *str)
{
	int		i;
	char	*result;

	if (!str)
		return (NULL);
	i = 0;
	result = safe_strdup("", shell);
	while (str[i])
	{
		if (str[i] == '$')
		{
			result = join_expanded_var(shell, result, str, &i);
		}
		else
			result = append_normal_char(result, str, &i, shell);
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
			if (cur->prev && cur->prev->type == HEREDOC)
			{
			}
			else
			{
				expanded = expand_variables(shell, cur->str);
				free(cur->str);
				cur->str = expanded;
			}
		}
		cur = cur->next;
	}
}
