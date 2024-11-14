/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 15:45:55 by aapadill          #+#    #+#             */
/*   Updated: 2024/11/14 15:45:57 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
	//not PATH found, not sure if this is how I should handle this
	ft_putstr_fd("./pipex: ", STDERR_FILENO);
	open("/non/existent", O_RDONLY);
	perror(cmd);
	gc_free_all();
	exit(EXIT_CMD_NOT_FOUND);
}

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
	//cmd not found in any of the paths
	ft_putstr_fd("./pipex: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putendl_fd(": command not found", STDERR_FILENO);
	gc_free_all();
	exit(EXIT_CMD_NOT_FOUND);
}

char	*get_cmd_path(char *cmd, char **envp)
{
	int		n;
	char	**paths;
	char	*path_env;
	char	*full_cmd_path;

	//no need to resolve path if cmd is already a valid path
	if (is_cmd_already_path(cmd))
		return (gc_strdup(cmd));

	//find path from envp
	path_env = find_path(envp, cmd);
	
	//splitting path into actual directories
	paths = gc_split(path_env, ':', &n);
	full_cmd_path = look_for_cmd(cmd, paths);
	return (full_cmd_path);
}
