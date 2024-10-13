/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 14:18:09 by aapadill          #+#    #+#             */
/*   Updated: 2024/05/13 17:54:51 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

int	ft_putstr(const char *s, int *count)
{
	int		i;
	char	*m;

	i = 0;
	m = "(null)";
	if (!s)
		while (i < 6)
			if (!ft_putchar(m[i++], count))
				return (0);
	while (s && *s)
	{
		if (ft_putchar(*s, count))
			s++;
		else
			return (0);
	}
	return (1);
}
