/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_char.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 16:04:04 by meel-war          #+#    #+#             */
/*   Updated: 2025/04/23 16:06:36 by meel-war         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_strjoin_char(char *s1, char c)
{
	char *res;
	int i;

	if(!s1)
		return(NULL);
	res = malloc(sizeof(char) * (ft_strlen(s1) + 2));
	if (!res)
		return(NULL);
	i = 0;
	while (s1[i])
	{
		res[i] = s1[i];
		i++;
	}
	res[i] = c;
	res[i + 1] = '\0';
	return(res);
}