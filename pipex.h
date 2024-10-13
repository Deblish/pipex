/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 13:31:05 by aapadill          #+#    #+#             */
/*   Updated: 2024/10/12 18:04:19 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h> //access()
# include <stdlib.h> //malloc()
# include <fcntl.h> //open()
# include <errno.h> //errno
# include <string.h> //strerror()
# include <stdio.h> //perror()

# include "libft/libft.h"
# include "ft_printf/include/ft_printf.h"

//utils
int	ft_perror(char *error_msg, int is_syscall);

#endif
