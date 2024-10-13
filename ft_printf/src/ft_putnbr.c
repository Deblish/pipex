/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 19:07:19 by aapadill          #+#    #+#             */
/*   Updated: 2024/05/16 11:51:42 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

int	ft_putnbr(long n, const char *base, int *count)
{
	char	base_n;

	base_n = (char)ft_strlen(base);
	if (n < 0)
	{
		if (!ft_putchar('-', count))
			return (0);
		n = -n;
	}
	if (n >= base_n)
		ft_putnbr(n / base_n, base, count);
	if (*count == -1 || !ft_putchar(base[n % base_n], count))
		return (0);
	return (1);
}
