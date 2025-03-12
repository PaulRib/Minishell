/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 16:00:15 by pribolzi          #+#    #+#             */
/*   Updated: 2025/03/12 17:54:47 by meel-war         ###   ########.fr       */
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
	if(place == true)
	{
		while (ptr->next)
			ptr = ptr->next;
		ptr->next = new;
		new->prev = ptr;
	}
	new->next = NULL;
}
