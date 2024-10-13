/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 11:19:26 by aapadill          #+#    #+#             */
/*   Updated: 2024/10/12 18:06:49 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <stdio.h>

char	*get_cmd_path(char *cmd, char **envp)
{
	int	i;
	int	n;
	char	**paths;
	char	*path_env;
	char	*full_path;

	//find path from envp
	path_env = NULL;
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path_env = envp[i] + 5; //hardcored jump PATH=
			break ;
		}
		i++;
	}
	if (!path_env)
		return (NULL); //ft_perror() would be better?

	//splitting path into actual directories
	paths = ft_split(path_env, ':', &n);
	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(full_path, cmd);
		if (access(full_path, F_OK | X_OK) == 0)
		{
			//free paths memory
			//free anything else?
			return (full_path);
		}
		free(full_path);
		i++;
	}
	//free paths memory
	return (NULL); //ft_perror() would be better?
}

/*
void	first_child(int infile, int outfile, int *pipefd, char **argv, char **envp)
{
	char	**cmd_args;
	char	*cmd_path;
	int	n;

	//standard input to infile
	if (dup2(infile, STDIN_FILENO) < 0)
		ft_perror("At dup2 infile", 1);
	//standard output to write end of the pipe
	if (dup2(pipefd[1], STDOUT_FILENO) < 0)
		ft_perror("At dup2 pipefd[1]", 1);
	close(pipefd[0]);
	close(infile);
	close(outfile);

	//parsing cmd
	cmd_args = ft_split(argv[2], ' ', &n);
	if (!cmd_args || !cmd_args[0])
		ft_perror("Invalid command", 0);
	//resolving cmd path
	cmd_path = get_cmd_path(cmd_args[0], envp);
	if (!cmd_path)
		ft_perror("Non-existent path", 0);
	if (execve(cmd_path, cmd_args, envp) == -1)
		ft_perror("At execve cmd 1", 0);
}
*/

//./pipex file1 cmd1 cmd2 file2
int	main (int argc, char **argv, char **envp)
{
	int	infile;
	int	outfile;
	int	pipefd[2];
	pid_t	pid1;
	pid_t	pid2;

	char	**cmd_args;
	char	*cmd_path;
	int	n;

	//error 1
	if (argc != 5)
		ft_perror("Too many little/many arguments", 0);
	infile = open(argv[1], O_RDONLY);
	outfile = open(argv[4], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (infile < 0 | outfile < 0)
		ft_perror("Error opening file", 1);
	if (pipe(pipefd) == -1)
		ft_perror("Pipe failed", 1);
	pid1 = fork();
	if (pid1 == 0)
	//	first_child(infile, outfile, &pipefd[2], argv, envp);
	{
		//standard input to infile
		if (dup2(infile, STDIN_FILENO) < 0)
			ft_perror("At dup2 infile", 1);
		//standard output to write end of the pipe
		if (dup2(pipefd[1], STDOUT_FILENO) < 0)
			ft_perror("At dup2 pipefd[1]", 1);
		close(pipefd[0]);
		close(infile);
		close(outfile);

		//parsing cmd
		cmd_args = ft_split(argv[2], ' ', &n);
		if (!cmd_args || !cmd_args[0])
			ft_perror("Invalid command", 0);
		//resolving cmd path
		cmd_path = get_cmd_path(cmd_args[0], envp);
		if (!cmd_path)
			ft_perror("Non-existent path", 0);
		if (execve(cmd_path, cmd_args, envp) == -1)
			ft_perror("At execve cmd 1", 0);
	}
	pid2 = fork();
	if (pid2 == 0)
		printf("2nd Child process: %i\n", pid2);
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	return (0);
}
