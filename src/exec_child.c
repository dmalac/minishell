/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_child.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/09 11:24:16 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/10/06 12:58:42 by dmalacov      ########   odam.nl         */
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

/* 
	This function processes all the tokens corresponding forming a particular 
	command and saves in the struct tools (i) the array of arguments, and 
	(ii) the correct fds for input and output. If the get_args function returned 
	EXIT_FAILURE becuase of a memory allocation error, the function exits.
 */
static void	st_get_tools(t_cmd_tools *tools, t_token_lst *input, \
int pipe_end[2][2])
{
	t_token_lst	*node;

	if (get_args(tools, input) == EXIT_FAILURE)
		child_error_and_exit(errno, tools, NULL);
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
}

/* 
	This function closes the appropriate file descriptors (fd) assigned to pipes 
	based on (i) at what stage of the process it is called (BEFORE or AFTER the 
	execution of the command), and (ii) whether there were pipes open for 
	heredoc.
 */
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

/* 
	This function verifies whether the command is a builtin function or not and
	performs execution accordingly. It returns either the exit code returned by 
	a builtin function (0 or 1) or -1 in case where there was an error executing 
	a command using the execve function.
 */
static int	st_execute_cmd(t_cmd_tools *tools, t_symtab *symtab)
{
	int		i;
	char	*full_cmd;
	char	*orig_cmd;

	if (is_builtin(tools->cmd_args[0]) == TRUE)
		return (execute_builtin(tools->cmd_args, symtab, CHILD));
	i = 0;
	while (tools->paths && tools->paths[i])
	{
		full_cmd = ft_strjoin(tools->paths[i++], tools->cmd_args[0]);
		if (access(full_cmd, F_OK) == 0)
		{
			orig_cmd = tools->cmd_args[0];
			tools->cmd_args[0] = full_cmd;
			if (execve(tools->cmd_args[0], tools->cmd_args, tools->env_var) < 0)
			{
				tools->cmd_args[0] = orig_cmd;
				return (free(full_cmd), -1);
			}
		}
		free (full_cmd);
	}
	if (contains_char(tools->cmd_args[0], '/') == 1)
		execve(tools->cmd_args[0], tools->cmd_args, tools->env_var);
	return (-1);
}

/* 
	This function manages everything that is necessary for the correct execution 
	of the command by a child process. It also handles the correct exit of the 
	child process in case (i) there was no command to execute, (ii) the command 
	was a builtin function or (iii) the execution of a command using execve 
	returned an error.
 */
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
	if (tools->cmd_args)
		exit_code = st_execute_cmd(tools, symtab);
	close_pipes_child(tools, pipe_end, AFTER);
	if (exit_code >= 0)
	{
		cleanup(tools);
		exit(exit_code);
	}
	if (access(tools->cmd_args[0], F_OK) < 0 || \
	contains_char(tools->cmd_args[0], '/') == 0)
		child_error_and_exit(CMD_ERROR, tools, tools->cmd_args[0]);
	else if (access(tools->cmd_args[0], X_OK) < 0)
		child_error_and_exit(errno, tools, tools->cmd_args[0]);
	else
		child_error_and_exit(errno, tools, tools->cmd_args[0]);
}
