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

	//open infile
	infile = open_infile(argv[1]);

	//redirect stdin to infile
	redirect_stdin(infile);

	//redirect stdout to write end of the pipe
	redirect_stdout(pipefd[1]);

	close(pipefd[0]);
	close(pipefd[1]);
	close(infile);

	//parsing cmd1
	cmd_args = parse_command(argv[2]);

	//resolving cmd path
	cmd_path = get_cmd_path(cmd_args[0], envp);

	//checking if cmd is a directory
	is_directory(cmd_path, cmd_args);

	//try exec
	try_exec(cmd_path, cmd_args, envp);

	//clean up
	gc_free_all();
}

void	second_child(int *pipefd, char **argv, char **envp)
{
	int		outfile;
	char	**cmd_args;
	char	*cmd_path;

	//open file
	outfile = open_outfile(argv[4]);

	//redirect stdin to read end of the pipe
	redirect_stdin(pipefd[0]);

	//redirect stdout to outfile
	redirect_stdout(outfile);
	
	close(pipefd[0]);
	close(pipefd[1]);
	close(outfile);

	//parsing cmd2
	cmd_args = parse_command(argv[3]);

	//resolving cmd path
	cmd_path = get_cmd_path(cmd_args[0], envp);

	//checking if cmd is a directory
	is_directory(cmd_path, cmd_args);

	//try exec
	try_exec(cmd_path, cmd_args, envp);

	//clean up
	gc_free_all();
}
