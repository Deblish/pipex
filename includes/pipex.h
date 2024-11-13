/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 13:31:05 by aapadill          #+#    #+#             */
/*   Updated: 2024/11/13 23:57:18 by aapadill         ###   ########.fr       */
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

# include "libft.h"
# include "gc_alloc.h"

# define EXIT_CMD_NOT_FOUND 127
# define EXIT_PERMISSION_DENIED 126

//utils
int		ft_perror(char *error_msg, int is_syscall);
void	ft_free(int n, void **ptr_array);
char	*get_cmd_path(char *cmd, char **envp);
void	ft_error(char *error_msg, int print_errno);
#endif
