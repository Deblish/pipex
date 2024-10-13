/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 14:26:28 by aapadill          #+#    #+#             */
/*   Updated: 2024/05/16 12:01:50 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# define HEX "0123456789abcdef"
# define DEC "0123456789"
# define HEX_CAPS "0123456789ABCDEF"
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include <stdarg.h>

size_t	ft_strlen(const char *s);
int		ft_putchar(const char c, int *count);
int		ft_putstr(const char *s, int *count);
int		ft_putnbr(long n, const char *base, int *count);
int		ft_putnbr_u(unsigned long n, const char *base, int *count);
int		ft_printf(const char *format, ...);

#endif
