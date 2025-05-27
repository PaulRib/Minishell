/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 17:41:46 by meel-war          #+#    #+#             */
/*   Updated: 2025/05/26 17:59:44 by meel-war         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*append_normal_char(char *result, char *str, int *i)
{
	char	c[2];
	char	*final;

	c[0] = str[*i];
	c[1] = '\0';
	final = ft_strjoin(result, c);
	free(result);
	(*i)++;
	return (final);
}
