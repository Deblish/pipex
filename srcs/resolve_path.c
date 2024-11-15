/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 15:45:55 by aapadill          #+#    #+#             */
/*   Updated: 2024/11/15 17:16:09 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
 ** Check if the command is already a valid path
 ** (need to resolve path if cmd is already a valid path)
 */
static int	is_cmd_already_path(char *cmd)
{
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) == 0)
			return (1);
		else
		{
			ft_putstr_fd("./pipex: ", STDERR_FILENO);
			perror(cmd);
			gc_free_all();
			exit(EXIT_CMD_NOT_FOUND);
		}
	}
	return (0);
}

/*
 ** Find the PATH variable in the environment
 ** If not found, print error message and exit
 */
static char	*find_path(char **envp, char *cmd)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	ft_putstr_fd("./pipex: ", STDERR_FILENO);
	open("/non/existent", O_RDONLY);
	perror(cmd);
	gc_free_all();
	exit(EXIT_CMD_NOT_FOUND);
}

/*
 ** Look for the command in all the paths, 
 ** if found, return the full path to the command
 ** if not found, print error message and exit
 */
static char	*look_for_cmd(char *cmd, char **paths)
{
	int		i;
	char	*aux;
	char	*triable_path;

	i = 0;
	while (paths[i])
	{
		aux = gc_strjoin(paths[i], "/");
		triable_path = gc_strjoin(aux, cmd);
		if (access(triable_path, F_OK | X_OK) == 0)
			return (triable_path);
		i++;
	}
	ft_putstr_fd("./pipex: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putendl_fd(": command not found", STDERR_FILENO);
	gc_free_all();
	exit(EXIT_CMD_NOT_FOUND);
}

char	*resolve_path(char *cmd, char **envp)
{
	int		n;
	char	**paths;
	char	*path_env;
	char	*full_cmd_path;

	if (is_cmd_already_path(cmd))
		return (gc_strdup(cmd));
	path_env = find_path(envp, cmd);
	paths = gc_split(path_env, ':', &n);
	full_cmd_path = look_for_cmd(cmd, paths);
	return (full_cmd_path);
}
