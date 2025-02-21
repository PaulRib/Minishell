/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 15:31:12 by pribolzi          #+#    #+#             */
/*   Updated: 2025/01/10 15:39:04 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*ptr;

	if (!content)
		return (NULL);
	ptr = malloc(sizeof(t_list));
	if (!ptr)
		return (NULL);
	ptr->content = content;
	ptr->next = NULL;
	return (ptr);
}
