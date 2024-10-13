/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_u.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 10:44:58 by aapadill          #+#    #+#             */
/*   Updated: 2024/05/16 11:57:00 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

int	ft_putnbr_u(unsigned long n, const char *base, int *count)
{
	unsigned char	base_n;

	base_n = ft_strlen(base);
	if (n >= base_n)
		ft_putnbr_u(n / base_n, base, count);
	if (*count == -1 || !ft_putchar(base[n % base_n], count))
		return (0);
	return (1);
}
