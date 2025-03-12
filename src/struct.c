/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 16:00:15 by pribolzi          #+#    #+#             */
/*   Updated: 2025/03/12 16:31:58 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	new_node_token(t_token *ptr)
{
	t_token	*new;

	if (!ptr)
		return ;
	new = malloc(sizeof(t_token));
	if (!new)
		return ;
	while (ptr->next)
		ptr = ptr->next;
	ptr->next = new;
	new->prev = ptr;
	new->next = NULL;
}
