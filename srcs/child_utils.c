/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 15:56:55 by aapadill          #+#    #+#             */
/*   Updated: 2024/11/14 15:56:56 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
 ** Redirects stdin to fd
 ** if it fails, prints an error message and exits
 */
void	redirect_stdin_to(int infile)
{
	if (dup2(infile, STDIN_FILENO) < 0)
	{
		perror("dup2 stdin");
		exit(EXIT_FAILURE);
	}
}

/*
 ** Redirects stdout to fd
 ** if it fails, prints an error message and exits
 */
void	redirect_stdout_to(int outfile)
{
	if (dup2(outfile, STDOUT_FILENO) < 0)
	{
		perror("dup2 stdout");
		exit(EXIT_FAILURE);
	}
}

/*
 ** Parses the command and returns an array of strings
 ** where the first element is the command and the rest are the arguments
 */
char	**parse_command(char *cmd)
{
	char	**cmd_args;
	int		n;

	cmd_args = gc_split(cmd, ' ', &n);
	if (!cmd_args || !cmd_args[0])
	{
		ft_putstr_fd("./pipex: ", STDERR_FILENO);
		if (cmd_args)
			ft_putstr_fd(cmd_args[0], STDERR_FILENO);
		else
			ft_putstr_fd(cmd, STDERR_FILENO);
		ft_putendl_fd(": command not found", STDERR_FILENO);
		gc_free_all();
		exit(EXIT_CMD_NOT_FOUND);
	}
	return (cmd_args);
}

/*
 ** Only checks if the command is a directory
 ** if it is, prints an error message and exits
 */
void	is_directory(char *cmd_path, char **cmd_args)
{
	char	*line;
	int		aux;

	line = NULL;
	aux = open(cmd_path, O_RDONLY);
	if (aux > 0)
	{
		line = gc_next_line(aux, READ_LINE);
		close(aux);
		if (!line)
		{
			ft_putstr_fd("./pipex: ", STDERR_FILENO);
			perror(cmd_args[0]);
			gc_free_all();
			exit(EXIT_PERMISSION_DENIED);
		}
	}
}

/*
 ** Executes the command if possible
 ** if not, prints an error message and exits
 */
void	try_exec(char *cmd_path, char **cmd_args, char **envp)
{
	if (execve(cmd_path, cmd_args, envp) == -1)
	{
		ft_putstr_fd("./pipex: ", STDERR_FILENO);
		perror(cmd_args[0]);
		gc_free_all();
		if (errno == EACCES)
			exit(EXIT_PERMISSION_DENIED);
		else if (errno == ENOENT)
			exit(EXIT_CMD_NOT_FOUND);
		else if (errno == EISDIR)
			exit(EXIT_PERMISSION_DENIED);
		else
			exit(EXIT_FAILURE);
	}
}
