/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 13:31:05 by aapadill          #+#    #+#             */
/*   Updated: 2024/11/14 16:00:58 by aapadill         ###   ########.fr       */
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

//child_processes.c
void	first_child(int *pipefd, char **argv, char **envp);
void	second_child(int *pipefd, char **argv, char **envp);

//child_utils.c
void	redirect_stdin_to(int infile);
void	redirect_stdout_to(int outfile);
char	**parse_command(char *cmd);
void	is_directory(char *cmd_path, char **cmd_args);
void	try_exec(char *cmd_path, char **cmd_args, char **envp);

//resolve_path.c
char	*resolve_path(char *cmd, char **envp);
#endif
