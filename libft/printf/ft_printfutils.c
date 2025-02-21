/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printfutils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 14:31:10 by pribolzi          #+#    #+#             */
/*   Updated: 2025/01/10 15:45:45 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

int	ft_putstr(char *s)
{
	int	i;

	i = 0;
	if (!s)
	{
		write (1, "(null)", 6);
		return (6);
	}
	while (s[i])
	{
		write (1, &s[i], 1);
		i++;
	}
	return (i);
}

int	ft_putnbr(int n)
{
	long int	nb;
	int			count;

	count = 0;
	nb = n;
	if (nb < 0)
	{
		count++;
		ft_putchar('-');
		nb *= -1;
	}
	if (nb > 9)
	{
		count += ft_putnbr (nb / 10);
		count += ft_putnbr (nb % 10);
	}
	else
	{
		count++;
		ft_putchar (nb + '0');
	}
	return (count);
}

int	ft_mini_putnbr(unsigned int nb)
{
	int	count;

	count = 0;
	if (nb > 9)
	{
		count += ft_putnbr (nb / 10);
		count += ft_putnbr (nb % 10);
	}
	else
	{
		count++;
		ft_putchar (nb + '0');
	}
	return (count);
}
