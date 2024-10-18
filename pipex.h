/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 13:31:05 by aapadill          #+#    #+#             */
/*   Updated: 2024/10/18 15:05:51 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h> //access() //fork()
# include <stdlib.h> //malloc()
# include <fcntl.h> //open()
# include <errno.h> //errno
# include <string.h> //strerror()
# include <stdio.h> //perror()
# include <sys/wait.h> //waitpid()

# include "libft/libft.h"
# include "ft_printf/include/ft_printf.h"

//utils
int	ft_perror(char *error_msg, int is_syscall);
void	ft_free(int n, void **ptr_array);
char	*get_cmd_path(char *cmd, char **envp);
#endif
