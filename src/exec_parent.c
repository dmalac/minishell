/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_parent.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/09 11:18:42 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/09/13 15:39:32 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include <errno.h>
#include <unistd.h>
#include <stdio.h>

int	pipe_and_fork(int *id, t_cmd_tools *tools, int (*pipe_end)[2])
{
	int	pipe_no;

	pipe_no = (tools->cmd % 2 == 0);
	if (tools->cmd < tools->total_cmds)
	{
		if (pipe(pipe_end[pipe_no]) < 0)
			return (errno);
	}
	*id = fork();
	if (*id < 0)
		return (errno);
	return (0);
}

void	close_unnecessary_pipes(t_cmd_tools *tools, int pipe_end[2][2])
{
	int			reading_pipe;
	int			writing_pipe;
	t_heredoc	*node;

	reading_pipe = ((tools->cmd - 1) % 2 == 0);
	writing_pipe = (tools->cmd % 2 == 0);
	if (tools->cmd < tools->total_cmds)
		close(pipe_end[writing_pipe][W]);
	if (tools->cmd > 1)
		close(pipe_end[reading_pipe][R]);
	if (tools->heredoc)
	{
		node = tools->heredoc;
		while (node && node->cmd_no <= tools->cmd)
		{
			if (node->cmd_no == tools->cmd)
				close(node->hd_pipe[R]);
			node = node->next;
		}
	}
}

int	wait_for_last_child(int id, size_t total_cmds)
{
	int	wait_status;
	int	exit_code;

	waitpid(id, &wait_status, 0);
	if (WIFEXITED(wait_status))
		exit_code = WEXITSTATUS(wait_status);
	else
		exit_code = 1;
	while (total_cmds > 1)
	{
		wait(&wait_status);
		total_cmds--;
	}
	return (exit_code);
}