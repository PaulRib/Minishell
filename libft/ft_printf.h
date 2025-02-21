/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 14:01:54 by pribolzi          #+#    #+#             */
/*   Updated: 2025/02/21 15:07:11 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include "libft.h"
# include <stdarg.h>

int	ft_printf(const char *format, ...) __attribute__ ((format(printf, 1, 2)));
int	ft_putchar(int c);
int	ft_putnbr(int n);
int	ft_putstr(char *s);
int	to_hexa(unsigned int c);
int	ft_mini_putnbr(unsigned int nb);
int	to_hexa_upper(unsigned int c);
int	ft_printp(unsigned long long c);

#endif