/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 16:00:15 by pribolzi          #+#    #+#             */
/*   Updated: 2025/03/13 16:54:40 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	new_node_token(t_token *ptr, bool place)
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
		new->next = ptr->next;
		ptr->next = new->prev;
		ptr->next = new;
	}
}
