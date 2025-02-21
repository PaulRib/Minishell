/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 16:58:46 by pribolzi          #+#    #+#             */
/*   Updated: 2025/01/10 15:33:55 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	ft_bzero(void *s, size_t n)
{
	unsigned char	*dst;
	size_t			i;

	if (!s)
		return ;
	i = 0;
	dst = (unsigned char *) s;
	while (i < n)
	{
		dst[i] = '\0';
		i++;
	}
}
