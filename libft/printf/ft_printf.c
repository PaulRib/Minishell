/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 14:01:51 by pribolzi          #+#    #+#             */
/*   Updated: 2025/01/10 15:45:47 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

int	ft_putchar(int c)
{
	write (1, &c, 1);
	return (1);
}

int	check_format(const char *format, va_list args)
{
	if (*format == 'c')
		return (ft_putchar(va_arg(args, int)));
	else if (*format == 's')
		return (ft_putstr(va_arg(args, char *)));
	else if (*format == 'p')
		return (ft_printp(va_arg(args, long long)));
	else if (*format == 'd' || *format == 'i')
		return (ft_putnbr(va_arg(args, int)));
	else if (*format == 'u')
		return (ft_mini_putnbr(va_arg(args, unsigned int)));
	else if (*format == 'x')
		return (to_hexa(va_arg(args, unsigned int)));
	else if (*format == 'X')
		return (to_hexa_upper(va_arg(args, unsigned int)));
	else if (*format == '%')
		return (ft_putchar('%'));
	return (0);
}

int	ft_printf(const char *format, ...)
{
	int		i;
	va_list	args;
	int		count;

	i = 0;
	count = 0;
	if (!format)
		return (-1);
	va_start (args, format);
	while (format[i])
	{
		if (format[i] == '%')
		{
			i++;
			count += check_format(&format[i], args);
		}
		else
			count += ft_putchar(format[i]);
		i++;
	}
	va_end(args);
	return (count);
}
