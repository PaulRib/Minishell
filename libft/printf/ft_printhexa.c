/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printhexa.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 13:33:47 by pribolzi          #+#    #+#             */
/*   Updated: 2025/01/10 15:45:42 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

int	to_hexa(unsigned int c)
{
	char	*base;
	int		count;

	count = 0;
	base = "0123456789abcdef";
	if (c >= 16)
	{
		count += to_hexa(c / 16);
		count += to_hexa(c % 16);
	}
	else
	{
		count++;
		ft_putchar(base[c]);
	}
	return (count);
}

int	to_hexa_upper(unsigned int c)
{
	char	*base;
	int		count;

	count = 0;
	base = "0123456789ABCDEF";
	if (c >= 16)
	{
		count += to_hexa_upper(c / 16);
		count += to_hexa_upper(c % 16);
	}
	else
	{
		count++;
		ft_putchar(base[c]);
	}
	return (count);
}

static int	to_hexa_ptr(unsigned long long c)
{
	char	*base;
	int		count;

	count = 0;
	base = "0123456789abcdef";
	if (c >= 16)
	{
		count += to_hexa_ptr(c / 16);
		count += to_hexa_ptr(c % 16);
	}
	else
	{
		count++;
		ft_putchar(base[c]);
	}
	return (count);
}

int	ft_printp(unsigned long long c)
{
	int	count;

	count = 0;
	if (c == 0)
	{
		write (1, "(nil)", 5);
		count = 5;
		return (count);
	}
	count += 2;
	write (1, "0x", 2);
	count += to_hexa_ptr(c);
	return (count);
}
