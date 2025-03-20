/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 16:00:15 by pribolzi          #+#    #+#             */
/*   Updated: 2025/03/20 15:43:21 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	new_node_token(t_token *ptr, bool place, t_token *target)
{
	t_token	*new;

	if (!ptr)
		return ;
	new = malloc(sizeof(t_token));
	if (!new)
		return ;
	new = ft_memset(new, 0, sizeof(t_token));
	if (place == true)
	{
		while (ptr->next)
			ptr = ptr->next;
		ptr->next = new;
		new->prev = ptr;
	}
	else if (place == false)
	{
		while (ptr != target)
			ptr = ptr->next;
		ptr->next = new;
		new->prev = ptr;
	}
}
