/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 17:29:51 by pribolzi          #+#    #+#             */
/*   Updated: 2025/01/10 15:35:34 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*dst;
	unsigned char	*srce;
	size_t			i;

	if (!dest || !src)
		return (NULL);
	dst = (unsigned char *) dest;
	srce = (unsigned char *) src;
	i = 0;
	while (i < n)
	{
		dst[i] = srce[i];
		i++;
	}
	return (dst);
}
