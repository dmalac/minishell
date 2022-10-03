/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/12 12:33:23 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/09/20 16:39:15 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"
#include "libft.h"
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <readline/readline.h>
#include <sys/wait.h>

int	heredoc_open_pipes(t_heredoc *hd_list)
{
	while (hd_list)
	{
		if (pipe(hd_list->hd_pipe) < 0)
		{
			ft_putendl_fd(strerror(errno), 2);
			return (EXIT_FAILURE);
		}
		hd_list = hd_list->next;
	}
	return (EXIT_SUCCESS);
}

void	heredoc_close_pipes(t_heredoc *hd_list)
{
	while (hd_list)
	{
		if (g_is_interupt == SIGINT)
			close(hd_list->hd_pipe[R]);
		close(hd_list->hd_pipe[W]);
		hd_list = hd_list->next;
	}
	g_is_interupt = 0;
}

void	heredoc_child_close_pipes(t_heredoc *hd_list, int end)
{
	while (hd_list)
	{
		close(hd_list->hd_pipe[end]);
		hd_list = hd_list->next;
	}
}
