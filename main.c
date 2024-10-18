/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 11:19:26 by aapadill          #+#    #+#             */
/*   Updated: 2024/10/18 17:41:32 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <stdio.h>

void	first_child(int *pipefd, char **argv, char **envp)
{
	int		infile;
	char	**cmd_args;
	char	*cmd_path;
	int		n;

	//parsing cmd
	cmd_args = ft_split(argv[2], ' ', &n);
	if (!cmd_args || !cmd_args[0])
		ft_perror("Invalid command 1st", 0);
	//resolving cmd path
	cmd_path = get_cmd_path(cmd_args[0], envp);
	if (!cmd_path)
	{
		perror(cmd_args[0]);
		free(cmd_path);
		ft_free(n, (void **)cmd_args);
		exit(127);
	}
	
	//redirecting infile
	infile = open(argv[1], O_RDONLY);
	if (infile < 0)
	{
		perror(argv[1]);
		exit(EXIT_FAILURE);
	}
	//stdin to infile
	if (dup2(infile, STDIN_FILENO) < 0)
	{
		printf("error at dup2 infile");
		//ft_perror("At dup2 infile", 1);
	}
	//stdout to write end of the pipe
	if (dup2(pipefd[1], STDOUT_FILENO) < 0)
	{
		printf("error at dup2 pipefd[1]");
		//ft_perror("At dup2 pipefd[1]", 1);
	}
	close(pipefd[0]);
	close(pipefd[1]);
	close(infile);

	if (execve(cmd_path, cmd_args, envp) == -1)
	{
		perror(cmd_args[0]);
		free(cmd_path);
		ft_free(n, (void **)cmd_args);
		exit(127);
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

	outfile = open(argv[4], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (outfile < 0)
	{
		perror(argv[4]);
		exit(EXIT_FAILURE);
	}
	//stdin to read end of the pipe
	if (dup2(pipefd[0], STDIN_FILENO) < 0)
		ft_perror("At dup2 pipefd[0]", 1);
	if (dup2(outfile, STDOUT_FILENO) < 0)
		ft_perror("At dup2 outfile", 1);
	close(pipefd[0]);
	close(pipefd[1]);
	close(outfile);

	//parsing cmd2
	cmd_args = ft_split(argv[3], ' ', &n);
	if (!cmd_args || !cmd_args[0])
	{
		ft_perror("Invalid command 2nd", 0);
	}
	//resolving cmd path
	cmd_path = get_cmd_path(cmd_args[0], envp);
	if (!cmd_path)
	{
		perror(cmd_args[0]);
		ft_free(n, (void **)cmd_args);
		exit(127);
	}
	if (execve(cmd_path, cmd_args, envp) == -1)
	{
		perror(cmd_args[0]);
		free(cmd_path);
		ft_free(n, (void **)cmd_args);
		exit(127);
	}
	free(cmd_path);
	ft_free(n, (void **)cmd_args);
}

//./pipex file1 cmd1 cmd2 file2
int	main (int argc, char **argv, char **envp)
{
	int	pipefd[2];
	pid_t	pid1;
	pid_t	pid2;
	int	status1;
	int	status2;

	int i = -1;
	while(envp[++i])
		printf("envp[%i]: %s\n", i, envp[i]);
	if (argc != 5)
		ft_perror("Too many little/many arguments", 0);
	if (pipe(pipefd) == -1)
		ft_perror("Pipe failed", 1);
	pid1 = fork();
	if (pid1 == -1)
		ft_perror("Fork failed", 1);
	if (pid1 == 0)
	{
		first_child(pipefd, argv, envp);
	}
	pid2 = fork();
	if (pid2 == -1)
		ft_perror("Fork failed", 1);
	if (pid2 == 0)
	{
		second_child(pipefd, argv, envp);
	}
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	if (WIFEXITED(status2))
		exit(WEXITSTATUS(status2));
	else
		exit(EXIT_FAILURE);
}
