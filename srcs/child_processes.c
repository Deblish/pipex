/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_processes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 15:58:09 by aapadill          #+#    #+#             */
/*   Updated: 2024/11/14 15:58:12 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
 ** Open the input file and return the file descriptor
 ** If the file cannot be opened, print an error message and exit
 */
static int	open_infile(char *infile)
{
	int	fd;

	fd = open(infile, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("./pipex: ", STDERR_FILENO);
		perror(infile);
		exit(EXIT_FAILURE);
	}
	return (fd);
}

/*
 ** Open the output file with write-only access.
 ** If the file does not exist,
 ** it will be created with read and write permissions for the owner (0644)
 ** If the file already exists, its contents will be TRUNC (cleared)
 */
static int	open_outfile(char *outfile)
{
	int	fd;

	fd = open(outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
	{
		ft_putstr_fd("./pipex: ", STDERR_FILENO);
		perror(outfile);
		exit(EXIT_FAILURE);
	}
	return (fd);
}

void	first_child(int *pipefd, char **argv, char **envp)
{
	int		infile;
	char	**cmd_args;
	char	*cmd_path;

	infile = open_infile(argv[1]);
	redirect_stdin_to(infile);
	redirect_stdout_to(pipefd[1]);
	close(pipefd[0]);
	close(pipefd[1]);
	close(infile);
	cmd_args = parse_command(argv[2]);
	cmd_path = resolve_path(cmd_args[0], envp);
	is_directory(cmd_path, cmd_args);
	try_exec(cmd_path, cmd_args, envp);
	gc_free_all();
}

void	second_child(int *pipefd, char **argv, char **envp)
{
	int		outfile;
	char	**cmd_args;
	char	*cmd_path;

	outfile = open_outfile(argv[4]);
	redirect_stdin_to(pipefd[0]);
	redirect_stdout_to(outfile);
	close(pipefd[0]);
	close(pipefd[1]);
	close(outfile);
	cmd_args = parse_command(argv[3]);
	cmd_path = resolve_path(cmd_args[0], envp);
	is_directory(cmd_path, cmd_args);
	try_exec(cmd_path, cmd_args, envp);
	gc_free_all();
}
