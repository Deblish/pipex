/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 13:03:40 by aapadill          #+#    #+#             */
/*   Updated: 2024/10/10 13:03:44 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	ft_perror(char *error_msg, int is_syscall)
{
	if (is_syscall)
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
	else
		ft_putendl_fd(error_msg, STDERR_FILENO);
	exit(EXIT_FAILURE);
}