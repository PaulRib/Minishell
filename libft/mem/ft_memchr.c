/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 16:05:47 by pribolzi          #+#    #+#             */
/*   Updated: 2025/01/10 15:35:30 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*str;
	unsigned char	c2;
	size_t			i;

	if (!s)
		return (NULL);
	i = 0;
	str = (unsigned char *) s;
	c2 = (unsigned char) c;
	while (i < n)
	{
		if (str[i] == c2)
			return (&str[i]);
		i++;
	}
	return (0);
}
