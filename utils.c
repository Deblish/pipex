/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 13:03:40 by aapadill          #+#    #+#             */
/*   Updated: 2024/10/18 17:49:04 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	ft_perror(char *error_msg, int is_syscall)
{
	if (is_syscall)
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
	else
		ft_putendl_fd(error_msg, STDERR_FILENO);
	exit(EXIT_FAILURE);
}

void	ft_free(int n, void **ptr_array)
{
	while (n--)
		free(ptr_array[n]);
	free(ptr_array);
}

char	*get_cmd_path(char *cmd, char **envp)
{
	int	i;
	int	n;
	char	**paths;
	char	*path_env;
	char	*aux_path;
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
		aux_path = ft_strjoin(paths[i], "/");
		free(paths[i]);
		full_path = ft_strjoin(aux_path, cmd);
		free(aux_path);
		if (access(full_path, F_OK | X_OK) == 0)
		{
			while (++i < n)
				free(paths[i]);
			free(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free(paths);
	//ft_printf("%s", stderr);
	//ft_printf("%s: command not found\n", cmd);
	return (NULL); //ft_perror() would be better?
}
