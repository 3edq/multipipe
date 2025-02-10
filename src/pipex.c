/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksaegusa <ksaegusa@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 14:24:17 by ksaegusa          #+#    #+#             */
/*   Updated: 2025/02/09 14:30:36 by ksaegusa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	open_file(char *filename, int mode)
{
	int	fd;

	if (mode == 0)
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (mode == 1)
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (mode == 2)
		fd = open(filename, O_RDONLY);
	else
		fd = -1;
	if (fd < 0)
		error_exit(filename);
	return (fd);
}

static void	child_exec(char *cmd, char **envp, int *prev_pipe_fd, int is_last,
		int out_fd, int *new_pipe)
{
	if (prev_pipe_fd[0] != -1)
	{
		if (dup2(prev_pipe_fd[0], STDIN_FILENO) == -1)
			error_exit("dup2");
		close(prev_pipe_fd[0]);
		close(prev_pipe_fd[1]);
	}
	if (!is_last)
	{
		close(new_pipe[0]);
		if (dup2(new_pipe[1], STDOUT_FILENO) == -1)
			error_exit("dup2");
		close(new_pipe[1]);
	}
	else
	{
		if (dup2(out_fd, STDOUT_FILENO) == -1)
			error_exit("dup2");
		close(out_fd);
	}
	execute(cmd, envp);
}

static void	child_process(char *cmd, char **envp, int *prev_pipe_fd,
		int is_last, int out_fd)
{
	int		new_pipe[2];
	pid_t	pid;

	if (!is_last && pipe(new_pipe) == -1)
		error_exit("pipe");
	pid = fork();
	if (pid < 0)
		error_exit("fork");
	if (pid == 0)
		child_exec(cmd, envp, prev_pipe_fd, is_last, out_fd, new_pipe);
	else
	{
		if (prev_pipe_fd[0] != -1)
		{
			close(prev_pipe_fd[0]);
			close(prev_pipe_fd[1]);
		}
		if (!is_last)
		{
			prev_pipe_fd[0] = new_pipe[0];
			prev_pipe_fd[1] = new_pipe[1];
		}
	}
}

static void	init_fds(int argc, char **argv, int *in_fd, int *out_fd)
{
	if (argc < 5)
		usage();
	*in_fd = open_file(argv[1], 2);
	*out_fd = open_file(argv[argc - 1], 1);
	if (dup2(*in_fd, STDIN_FILENO) == -1)
		error_exit("dup2");
	close(*in_fd);
}

int	main(int argc, char **argv, char **envp)
{
	int	infile_fd;
	int	outfile_fd;
	int	pipe_fd[2];
	int	is_last;
	int	i;

	init_fds(argc, argv, &infile_fd, &outfile_fd);
	pipe_fd[0] = -1;
	pipe_fd[1] = -1;
	i = 2;
	while (i < argc - 1)
	{
		is_last = (i == (argc - 2));
		child_process(argv[i], envp, pipe_fd, is_last, outfile_fd);
		i++;
	}
	if (pipe_fd[0] != -1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
	close(outfile_fd);
	while (wait(NULL) > 0)
		;
	return (0);
}
