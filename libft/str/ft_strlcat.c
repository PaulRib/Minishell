/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 13:35:08 by pribolzi          #+#    #+#             */
/*   Updated: 2025/01/10 15:36:03 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	dst_size;
	size_t	src_size;

	if (!dst || !src)
		return (0);
	i = 0;
	src_size = ft_strlen(src);
	if (!dst && size == 0)
		return (ft_strlen(src));
	dst_size = ft_strlen(dst);
	while (*dst != '\0' && dst_size < size)
		dst++;
	if (size <= dst_size || size <= 0)
		return (src_size + size);
	while (src[i] != '\0' && dst_size + i + 1 < size)
	{
		*dst = src[i];
		dst++;
		i++;
	}
	*dst = '\0';
	return (dst_size + src_size);
}
