/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 17:41:46 by meel-war          #+#    #+#             */
/*   Updated: 2025/05/28 17:06:30 by meel-war         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*append_normal_char(char *result, char *str, int *i, t_shell *shell)
{
	char	c[2];
	char	*final;

	c[0] = str[*i];
	c[1] = '\0';
	final = ft_strjoin(result, c);
	if (!final)
	{
		free(result);
		free_all(shell, 1);
	}
	free(result);
	(*i)++;
	return (final);
}

char	*join_expanded_var(t_shell *shell, char *result, char *str, int *i)
{
	char	*var;
	char	*tmp;

	var = handle_dollar(shell, str, i);
	tmp = result;
	result = ft_strjoin(result, var);
	if (!result)
	{
		free(var);
		free(tmp);
		free_all(shell, 1);
	}
	free(tmp);
	free(var);
	return (result);
}
