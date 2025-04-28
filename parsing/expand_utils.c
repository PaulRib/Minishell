/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 17:41:46 by meel-war          #+#    #+#             */
/*   Updated: 2025/04/28 17:42:20 by meel-war         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	update_result(char **result, char *var)
{
	char	*tmp;

	tmp = *result;
	*result = ft_strjoin(*result, var);
	free(tmp);
}
void	append_normal_char(char **result, char *str, int *i)
{
	char c[2];
	char *tmp;

	c[0] = str[*i];
	c[1] = '\0';
	tmp = *result;
	*result = ft_strjoin(*result, c);
	free(tmp);
	(*i)++;
}