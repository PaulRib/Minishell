/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:01:22 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/19 17:22:44 by meel-war         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*give_curr_cmd(t_shell *shell, int i)
{
	char	*str;
	t_token	*current;

	str = ft_calloc(1, 1);
	current = shell->token;
	while (current && i)
	{
		if (current->type == PIPE)
			i--;
		current = current->next;
	}
	while (current && current->type != PIPE)
	{
		if (current->type == WORD || current->type == CMD)
			str = ft_strjoin(str, current->str);
		if (current->next && current->next->type == WORD)
			str = ft_strjoin(str, " ");
		current = current->next;
	}
	return (str);
}