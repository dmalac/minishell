/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_child.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/09 11:24:16 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/09/26 15:55:04 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "executor.h"
#include "symtab.h"
#include "builtin.h"
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

static char	*st_prepare_cmd(char *cmd, char **paths)
{
	int		i;
	char	*full_cmd;

	if (access(cmd, F_OK) == 0)
		return (cmd);
	i = 0;
	while (paths && paths[i])
	{
		full_cmd = ft_strjoin(paths[i++], cmd);
		if (access(full_cmd, F_OK) == 0)
		{
			free(cmd);
			return (full_cmd);
		}
		else
			free (full_cmd);
	}
	return (cmd);
}

static void	st_get_tools(t_cmd_tools *tools, t_token_lst *input, \
int pipe_end[2][2])
{
	t_token_lst	*node;

	get_args(tools, input);
	node = input;
	while (node && node->token_type != PIPE && tools->input_fd != -1 && \
	tools->output_fd != -1)
		node = tools->process_tokens[node->token_type](tools, node);
	if (tools->input_fd == -1 || tools->output_fd == -1)
		return ;
	if (tools->input_fd == STDIN_FILENO && tools->total_cmds > 1 && \
	tools->cmd > 1)
		tools->input_fd = pipe_end[tools->cmd % 2 == 1][R];
	if (tools->output_fd == STDOUT_FILENO && tools->total_cmds > 1 && \
	tools->cmd != tools->total_cmds)
		tools->output_fd = pipe_end[tools->cmd % 2 == 0][W];
	if (tools->cmd_args && is_builtin(tools->cmd_args[0]) == 0)
		tools->cmd_args[0] = st_prepare_cmd(tools->cmd_args[0], tools->paths);
}

static void	close_pipes_child(t_cmd_tools *tools, int pipe_end[2][2], int when)
{
	t_heredoc	*node;

	if (when == BEFORE && tools->cmd < tools->total_cmds)
		close(pipe_end[tools->cmd % 2 == 0][R]);
	if (when == AFTER)
	{
		if (tools->cmd < tools->total_cmds)
			close(pipe_end[tools->cmd % 2 == 0][W]);
		if (tools->cmd > 1)
			close(pipe_end[(tools->cmd - 1) % 2 == 0][R]);
	}
	if (tools->heredoc)
	{
		node = tools->heredoc;
		while (node)
		{
			if (when == BEFORE && node->cmd_no > tools->cmd)
				close(node->hd_pipe[R]);
			if (when == AFTER && node->cmd_no == tools->cmd)
				close(node->hd_pipe[R]);
			node = node->next;
		}
	}
}

void	perform_cmd(t_cmd_tools *tools, t_token_lst *input, int pipe_end[2][2], \
t_symtab *symtab)
{
	int	exit_code;

	exit_code = 0;
	st_get_tools(tools, input, pipe_end);
	if (tools->input_fd < 0 || tools->output_fd < 0)
		child_error_and_exit(IO_FD_ERROR, tools, NULL);
	close_pipes_child(tools, pipe_end, BEFORE);
	dup2(tools->input_fd, STDIN_FILENO);
	dup2(tools->output_fd, STDOUT_FILENO);
	if (!tools->cmd_args)
		child_error_and_exit(NO_ERROR, tools, NULL);
	if (is_builtin(tools->cmd_args[0]) > 0)
		exit_code = execute_builtin(tools->cmd_args, symtab);
	else if (execve(tools->cmd_args[0], tools->cmd_args, tools->env_var) < 0)
	{
		close_pipes_child(tools, pipe_end, AFTER);
		if (access(tools->cmd_args[0], F_OK) < 0)
			child_error_and_exit(CMD_ERROR, tools, tools->cmd_args[0]);
		else if (access(tools->cmd_args[0], X_OK) < 0)
			child_error_and_exit(errno, tools, tools->cmd_args[0]);
	}
	close_pipes_child(tools, pipe_end, AFTER);
	child_error_and_exit(exit_code, tools, NULL);
}
