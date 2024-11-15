/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 11:19:26 by aapadill          #+#    #+#             */
/*   Updated: 2024/11/10 13:10:52 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	ft_error(char *msg)
{
	ft_putendl_fd(msg, STDERR_FILENO);
	exit(EXIT_FAILURE);
}

static void	wait_and_exit(pid_t pid1, pid_t pid2)
{
	int	status1;
	int	status2;

	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	if (WIFEXITED(status2))
		exit(WEXITSTATUS(status2));
	else
		exit(EXIT_FAILURE);
}

int	main (int argc, char **argv, char **envp)
{
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;

	if (argc != 5)
		ft_error("Usage: ./pipex file1 cmd1 cmd2 file2");
	if (pipe(pipefd) == -1)
		ft_error("./pipex: pipe failed");
	pid1 = fork();
	if (pid1 == -1)
		ft_error("./pipex: fork 1 failed");
	if (pid1 == 0)
		first_child(pipefd, argv, envp);
	pid2 = fork();
	if (pid2 == -1)
		ft_error("./pipex: fork 2 failed");
	if (pid2 == 0)
		second_child(pipefd, argv, envp);
	close(pipefd[0]);
	close(pipefd[1]);
	wait_and_exit(pid1, pid2);
}
