/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 11:19:26 by aapadill          #+#    #+#             */
/*   Updated: 2024/10/24 15:55:23 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <stdio.h>

void	first_child(int *pipefd, char **argv, char **envp)
{
	int	infile;
	char	**cmd_args;
	char	*cmd_path;
	int	n;

	//open infile
	infile = open(argv[1], O_RDONLY);
	if (infile < 0)
	{
		ft_putstr_fd("./pipex: ", STDERR_FILENO);
		perror(argv[1]);
		exit(EXIT_FAILURE);
		//infile = open("/dev/null", O_RDONLY);
	}

	//redirect stdin to infile
	if (dup2(infile, STDIN_FILENO) < 0)
		perror("dup2 infile");

	//redirect stdout to write end of the pipe
	if (dup2(pipefd[1], STDOUT_FILENO) < 0)
		perror("dup2 pipefd[1");

	close(pipefd[0]);
	close(pipefd[1]);
	close(infile);

	//parsing cmd1
	cmd_args = ft_split(argv[2], ' ', &n);
	if (!cmd_args || !cmd_args[0])
	{
		ft_putstr_fd("./pipex: ", STDERR_FILENO);
		if (cmd_args)
			ft_putstr_fd(cmd_args[0], STDERR_FILENO);
		else
			ft_putstr_fd(argv[2], STDERR_FILENO);
		ft_putendl_fd(": command not found", STDERR_FILENO);
		ft_free(n, (void **)cmd_args);
		exit(127);
	}

	//resolving cmd path
	cmd_path = get_cmd_path(cmd_args[0], envp);
	if (!cmd_path)
	{
		ft_putstr_fd("./pipex: ", STDERR_FILENO);
		ft_putstr_fd(cmd_args[0], STDERR_FILENO);
		ft_putendl_fd(": command not found", STDERR_FILENO);
		ft_free(n, (void **)cmd_args);
		exit(127);
	}

	//checking if cmd is a directory
	char *line = NULL;
	int aux = open(cmd_path, O_RDONLY);
	if (aux > 0)
	{
		line = get_next_line(aux);
		close(aux);
		if (!line)
		{
			ft_putstr_fd("./pipex: ", STDERR_FILENO);
			perror(cmd_args[0]);
			free(cmd_path);
			ft_free(n, (void **)cmd_args);
			exit(126);
		}
	}

	//try exec
	if (execve(cmd_path, cmd_args, envp) == -1)
	{
		ft_putstr_fd("./pipex: ", STDERR_FILENO);
		perror(cmd_args[0]);
		free(cmd_path);
		ft_free(n, (void **)cmd_args);
		if (errno == EACCES)
			exit(126);
		else if (errno == ENOENT)
			exit(127);
		else if (errno == EISDIR)
			exit(126);
		else
			exit(1);
	}
	free(cmd_path);
	ft_free(n, (void **)cmd_args);
}

void	second_child(int *pipefd, char **argv, char **envp)
{
	int		outfile;
	char	**cmd_args;
	char	*cmd_path;
	int		n;

	//open file
	outfile = open(argv[4], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (outfile < 0)
	{
		ft_putstr_fd("./pipex: ", STDERR_FILENO);
		perror(argv[4]);
		exit(1);
	}

	//redirect stdin to read end of the pipe
	if (dup2(pipefd[0], STDIN_FILENO) < 0)
		perror("dup2 pipefd[0]");

	//redirect stdout to outfile
	if (dup2(outfile, STDOUT_FILENO) < 0)
		perror("dup2 outfile");
	close(pipefd[0]);
	close(pipefd[1]);
	close(outfile);

	//parsing cmd2
	cmd_args = ft_split(argv[3], ' ', &n);
	if (!cmd_args || !cmd_args[0])
	{
		ft_putstr_fd("./pipex: ", STDERR_FILENO);
		if (cmd_args)
			ft_putstr_fd(cmd_args[0], STDERR_FILENO);
		else
			ft_putstr_fd(argv[3], STDERR_FILENO);
		ft_putendl_fd(": command not found", STDERR_FILENO);
		ft_free(n, (void **)cmd_args);
		exit(127);
	}

	//resolving cmd path
	cmd_path = get_cmd_path(cmd_args[0], envp);
	if (!cmd_path)
	{
		ft_putstr_fd("./pipex: ", STDERR_FILENO);
		ft_putstr_fd(cmd_args[0], STDERR_FILENO);
		ft_putendl_fd(": command not found", STDERR_FILENO);
		ft_free(n, (void **)cmd_args);
		exit(127);
	}

	//checking if cmd is a directory
	char *line = NULL;
	int aux = open(cmd_path, O_RDONLY);
	if (aux > 0)
	{
		line = get_next_line(aux);
		close(aux);
		if (!line)
		{
			ft_putstr_fd("./pipex: ", STDERR_FILENO);
			perror(cmd_args[0]);
			free(cmd_path);
			ft_free(n, (void **)cmd_args);
			exit(126);
		}
	}

	//try exec
	if (execve(cmd_path, cmd_args, envp) == -1)
	{
		ft_putstr_fd("./pipex: ", STDERR_FILENO);
		perror(cmd_args[0]);
		free(cmd_path);
		ft_free(n, (void **)cmd_args);
		if (errno == EACCES)
			exit(126);
		else if (errno == ENOENT)
			exit(127);
		else if (errno == EISDIR)
			exit(126);
		else
			exit(1);
	}
	free(cmd_path);
	ft_free(n, (void **)cmd_args);
}

//./pipex file1 cmd1 cmd2 file2
int	main (int argc, char **argv, char **envp)
{
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status1;
	int		status2;

	if (argc != 5)
	{
		ft_putendl_fd("Usage: ./pipex file1 cmd1 cmd2 file2", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	if (pipe(pipefd) == -1)
		ft_perror("Pipe failed", 1);
	pid1 = fork();
	if (pid1 == -1)
		ft_perror("Fork failed", 1);
	if (pid1 == 0)
		first_child(pipefd, argv, envp);
	pid2 = fork();
	if (pid2 == -1)
		ft_perror("Fork failed", 1);
	if (pid2 == 0)
		second_child(pipefd, argv, envp);
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	if (WIFEXITED(status2))
		exit(WEXITSTATUS(status2));
	else
		exit(EXIT_FAILURE);
}
