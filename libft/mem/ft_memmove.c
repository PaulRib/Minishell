/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 16:00:57 by pribolzi          #+#    #+#             */
/*   Updated: 2025/01/10 15:35:39 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*dst;
	unsigned char	*srce;
	size_t			i;

	if (!dest || !src)
		return (NULL);
	dst = (unsigned char *) dest;
	srce = (unsigned char *) src;
	if (dst > srce)
	{
		i = n;
		while (i > 0)
		{
			i--;
			dst[i] = srce[i];
		}
	}
	else
		ft_memcpy(dest, src, n);
	return (dst);
}
