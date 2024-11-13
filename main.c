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

int	open_infile(char *infile)
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

int	open_outfile(char *outfile)
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

void	redirect_stdin(int infile)
{
	if (dup2(infile, STDIN_FILENO) < 0)
	{
		perror("dup2 stdin");
		exit(EXIT_FAILURE);
	}
}

void	redirect_stdout(int outfile)
{
	if (dup2(outfile, STDOUT_FILENO) < 0)
	{
		perror("dup2 stdout");
		exit(EXIT_FAILURE);
	}
}

char	**parse_command(char *cmd, int *n)
{
	char **cmd_args;

	cmd_args = ft_split(cmd, ' ', n);
	if (!cmd_args || !cmd_args[0])
	{
		ft_putstr_fd("./pipex: ", STDERR_FILENO);
		if (cmd_args)
			ft_putstr_fd(cmd_args[0], STDERR_FILENO);
		else
			ft_putstr_fd(cmd, STDERR_FILENO);
		ft_putendl_fd(": command not found", STDERR_FILENO);
		ft_free(*n, (void **)cmd_args);
		exit(EXIT_CMD_NOT_FOUND);
	}
	return (cmd_args);
}

char	*resolve_cmd_path(char **cmd_args, char **envp, int *n)
{
	char	*cmd_path;

	cmd_path = get_cmd_path(cmd_args[0], envp);
	if (!cmd_path)
	{
		ft_putstr_fd("./pipex: ", STDERR_FILENO);
		ft_putstr_fd(cmd_args[0], STDERR_FILENO);
		ft_putendl_fd(": command not found", STDERR_FILENO);
		ft_free(*n, (void **)cmd_args);
		exit(EXIT_CMD_NOT_FOUND);
	}
	return (cmd_path);
}

void is_directory(char *cmd_path, char **cmd_args, int n)
{
	char	*line;
	int		aux;

	line = NULL;
	aux = open(cmd_path, O_RDONLY);
	if (aux > 0)
	{
		line = get_next_line(aux, READ_LINE);
		close(aux);
		if (!line)
		{
			ft_putstr_fd("./pipex: ", STDERR_FILENO);
			perror(cmd_args[0]);
			free(cmd_path);
			ft_free(n, (void **)cmd_args);
			exit(EXIT_PERMISSION_DENIED);
		}
		free(line);
	}
}

void	try_exec(char *cmd_path, char **cmd_args, char **envp, int n)
{
	if (execve(cmd_path, cmd_args, envp) == -1)
	{
		ft_putstr_fd("./pipex: ", STDERR_FILENO);
		perror(cmd_args[0]);
		free(cmd_path);
		ft_free(n, (void **)cmd_args);
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

void	first_child(int *pipefd, char **argv, char **envp)
{
	int		infile;
	char	**cmd_args;
	char	*cmd_path;
	int		n;

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
	cmd_args = parse_command(argv[2], &n);

	//resolving cmd path
	cmd_path = resolve_cmd_path(cmd_args, envp, &n);

	//checking if cmd is a directory
	is_directory(cmd_path, cmd_args, n);

	//try exec
	try_exec(cmd_path, cmd_args, envp, n);

	//clean up
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
	outfile = open_outfile(argv[4]);

	//redirect stdin to read end of the pipe
	redirect_stdin(pipefd[0]);

	//redirect stdout to outfile
	redirect_stdout(outfile);
	
	close(pipefd[0]);
	close(pipefd[1]);
	close(outfile);

	//parsing cmd2
	cmd_args = parse_command(argv[3], &n);

	//resolving cmd path
	cmd_path = resolve_cmd_path(cmd_args, envp, &n);

	//checking if cmd is a directory
	is_directory(cmd_path, cmd_args, n);

	//try exec
	try_exec(cmd_path, cmd_args, envp, n);

	//clean up
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
	{
		ft_putstr_fd("./pipex: pipe failed", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	pid1 = fork();
	if (pid1 == -1)
	{
		ft_putstr_fd("./pipex: fork 1 failed", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	if (pid1 == 0)
		first_child(pipefd, argv, envp);
	pid2 = fork();
	if (pid2 == -1)
	{
		ft_putstr_fd("./pipex: fork 2 failed", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
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

