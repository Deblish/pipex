/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 13:03:40 by aapadill          #+#    #+#             */
/*   Updated: 2024/11/01 11:38:28 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_free(int n, void **ptr_array)
{
	while (n--)
		free(ptr_array[n]);
	free(ptr_array);
}

char	*get_cmd_path(char *cmd, char **envp)
{
	int		i;
	int		n;
	char	**paths;
	char	*path_env;
	char	*aux_path;
	char	*full_path;

	//no need to resolve path if cmd is already a valid path
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) == 0)
			return (gc_strdup(cmd));
		else
		{
			ft_putstr_fd("./pipex: ", STDERR_FILENO);
			perror(cmd);
			gc_free_all();
			exit(EXIT_CMD_NOT_FOUND);
		}
	}

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
	{
		ft_putstr_fd("./pipex: ", STDERR_FILENO);
		open("/non/existent", O_RDONLY);
		perror(cmd);
		exit(EXIT_CMD_NOT_FOUND);
	}

	//splitting path into actual directories
	paths = gc_split(path_env, ':', &n);
	i = 0;
	while (paths[i])
	{
		aux_path = gc_strjoin(paths[i], "/");
		//free(paths[i]);
		full_path = gc_strjoin(aux_path, cmd);
		//free(aux_path);
		if (access(full_path, F_OK | X_OK) == 0)
		{
			//while (++i < n)
			//	free(paths[i]);
			//free(paths);
			return (full_path);
		}
		//free(full_path);
		i++;
	}
	//free(paths);
	return (NULL);
}
