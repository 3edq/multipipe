/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksaegusa <ksaegusa@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 14:24:47 by ksaegusa          #+#    #+#             */
/*   Updated: 2025/02/04 17:22:31 by ksaegusa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

void	error_exit(const char *msg);
char	*find_path(char *cmd, char **envp);
void	execute(char *argv, char **envp);
void	free_tab(char **tab);
int		open_file(char *filename, int mode);
void	here_doc(char *limiter);
void	usage(void);
char	*get_next_line(int fd);

#endif
