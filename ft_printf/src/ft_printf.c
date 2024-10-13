/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 20:33:52 by aapadill          #+#    #+#             */
/*   Updated: 2024/05/16 12:01:31 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** @description
** see man printf
**
** @param
** format: string with format to print, includes many escape sequences, flags
** and format characters
** arguments: va_list with the elements of the format characters
**
** @return
** number of characters printed, negative if error
*/

#include "../include/ft_printf.h"

static int	ft_printf_helper(const char *c, va_list args, int *i)
{
	return ((*c == 'c' && !ft_putchar(va_arg(args, int), i))
		|| (*c == 's' && !ft_putstr(va_arg(args, char *), i))
		|| (*c == 'p' && (!ft_putstr("0x", i)
				|| !ft_putnbr_u(va_arg(args, unsigned long), HEX, i)))
		|| (*c == 'd' && !ft_putnbr(va_arg(args, int), DEC, i))
		|| (*c == 'i' && !ft_putnbr(va_arg(args, int), DEC, i))
		|| (*c == 'u' && !ft_putnbr(va_arg(args, unsigned int), DEC, i))
		|| (*c == 'x' && !ft_putnbr(va_arg(args, unsigned int), HEX, i))
		|| (*c == 'X' && !ft_putnbr(va_arg(args, unsigned int), HEX_CAPS, i))
		|| (*c == '%' && !ft_putchar('%', i)));
}

int	ft_printf(const char *format, ...)
{
	va_list		args;
	int			i;

	i = 0;
	va_start(args, format);
	while (*format)
	{
		if (*format == '%' && *(format + 1) && format++)
		{
			if (ft_printf_helper(format, args, &i))
				break ;
		}
		else if (!ft_putchar(*format, &i))
			break ;
		format++;
	}
	va_end(args);
	return (i);
}
