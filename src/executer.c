/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksaegusa <ksaegusa@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 14:24:24 by ksaegusa          #+#    #+#             */
/*   Updated: 2025/02/05 19:54:31 by ksaegusa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void usage(void)
{
    ft_putstr_fd("Usage:\n", STDERR_FILENO);
    ft_putstr_fd("  ./pipex infile cmd1 cmd2 ... cmdN outfile\n", STDERR_FILENO);
    ft_putstr_fd("  ./pipex here_doc LIMITER cmd1 cmd2 ... cmdN outfile\n", STDERR_FILENO);
    exit(EXIT_FAILURE);
}

static char	*build_path(char *dir, char *cmd)
{
	char	*path;
	char	*full_path;

	if (!dir)
	{
		if (access(cmd, F_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path = ft_strjoin(dir, "/");
	if (!path)
		return (NULL);
	full_path = ft_strjoin(path, cmd);
	free(path);
	if (full_path && access(full_path, F_OK) == 0)
		return (full_path);
	free(full_path);
	return (NULL);
}

char	*find_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	int		i;

	if (cmd[0] == '/' || (cmd[0] == '.' && (cmd[1] == '/' || cmd[1] == '.')))
		return (build_path(NULL, cmd));
	while (*envp && ft_strnstr(*envp, "PATH", 4) == 0)
		envp++;
	if (!*envp)
		return (NULL);
	paths = ft_split(*envp + 5, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		path = build_path(paths[i++], cmd);
		if (path)
		{
			free_tab(paths);
			return (path);
		}
	}
	free_tab(paths);
	return (NULL);
}

void error_exit(const char *msg)
{
    if (msg && *msg)
        perror(msg);
    else
        perror("Error");
    exit(EXIT_FAILURE);
}

void	free_tab(char **tab)
{
	int	i;

	if (!tab)
		return ;
	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

void execute(char *cmd_str, char **envp)
{
    char    **cmd;
    char    *path;

    cmd = ft_split(cmd_str, ' ');
    if (!cmd || !cmd[0])
    {
        ft_putstr_fd("command not found\n", STDERR_FILENO);
        free_tab(cmd);
        exit(127);
    }
    path = find_path(cmd[0], envp);
    if (!path)
    {
        ft_putstr_fd(cmd[0], STDERR_FILENO);
        ft_putstr_fd(": command not found\n", STDERR_FILENO);
        free_tab(cmd);
        exit(127);
    }
    execve(path, cmd, envp);
    perror(cmd[0]);
    free(path);
    free_tab(cmd);
    exit(EXIT_FAILURE);
}
