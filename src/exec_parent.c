/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_parent.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/09 11:18:42 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/09/20 17:50:33 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "builtin.h"
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int	pipe_and_fork(int *id, t_cmd_tools *tools, int (*pipe_end)[2])
{
	int	pipe_no;

	pipe_no = (tools->cmd % 2 == 0);
	if (tools->cmd < tools->total_cmds)
	{
		if (pipe(pipe_end[pipe_no]) < 0)
		{
			strerror(errno);
			return (1);
		}
	}
	if (tools->total_cmds == 1 && tools->builtin_only == 1)
		return (-1);
	else
	{
		*id = fork();
		if (*id < 0)
		{
			strerror(errno);
			return (1);
		}
	}
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

static void	st_get_tools(t_cmd_tools *tools, t_token_lst *input)
{
	t_token_lst	*node;

	get_args(tools, input);
	node = input;
	while (node && node->token_type != PIPE && tools->input_fd != -1 && \
	tools->output_fd != -1)
		node = tools->process_tokens[node->token_type](tools, node);
}

int	parent_exec_builtin(t_cmd_tools *tools, t_token_lst *input, \
t_symtab *symtab)
{
	st_get_tools(tools, input);
	if (tools->input_fd < 0 || tools->output_fd < 0)
		return (1);
	dup2(tools->input_fd, STDIN_FILENO);
	dup2(tools->output_fd, STDOUT_FILENO);
	return (execute_builtin(tools->cmd_args, symtab));
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