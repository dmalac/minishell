/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_parent.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/09 11:18:42 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/10/18 18:55:46 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "builtin.h"
#include "libft.h"
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

/* 
	This function assesses whether it is necessary to open a pipe and fork the
	process in order to perform all the commands contained in the input based on 
	the information contained in the initialised struct tools.
	If there is only one command, no pipe is required. Whether the process will 
	be forked, depends on whether the command is a builtin function. 
	If it is a 	builtin function, the process is not forked and -1 is returned.
	If it is a command that will have to be executed using the execve function, 
	a child process is created.
 */
int	ex_pipe_and_fork(int *id, t_cmd_tools *tools, int (*pipe_end)[2])
{
	int	pipe_no;

	if (tools->total_cmds == 1 && tools->builtin_only == 1)
		return (-1);
	pipe_no = (tools->cmd % 2 == 0);
	if (tools->cmd < tools->total_cmds)
	{
		if (pipe(pipe_end[pipe_no]) < 0)
		{
			ex_print_error_message(errno, "pipe");
			return (EXIT_FAILURE);
		}
	}
	*id = fork();
	if (*id < 0)
	{
		ex_print_error_message(errno, "fork");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

/* 
	This function closes the ends of pipes in the parent process which will no 
	longer be needed. In particular, it closes 
	(i) the writing [W] end of the pipe that was newly opened in this round of 
	the loop in the executor function (since the child process that will read
	from that pipe still needs to be created and so the [R] end needs to remain
	open), and 
	(ii) the reading [R] end of the pipe taht was opened in the previous round
	of the loop in the executor function (since the child process that will read
	from that pipe has already been created).
	
 */
void	ex_close_unnecessary_pipes(t_cmd_tools *tools, int pipe_end[2][2])
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

/* 
	The function processes all tokens that belong to the to be executed builtin
	function, takes care of redirections and creates an array of arguments.
 */
static int	st_get_tools(t_cmd_tools *tools, t_token_lst *input)
{
	t_token_lst	*node;

	if (ex_get_args(tools, input) == EXIT_FAILURE)
		return (ft_putendl_fd(strerror(errno), STDERR_FILENO), EXIT_FAILURE);
	node = input;
	while (node && node->token_type != PIPE && tools->input_fd != -1 && \
	tools->output_fd != -1)
		node = tools->process_tokens[node->token_type](tools, node);
	return (EXIT_SUCCESS);
}

/* 
	This function executes a builtin function and returns the exit code that the
	builtin function returned.
 */
int	ex_parent_exec_builtin(t_cmd_tools *tools, t_token_lst *input, \
t_symtab **symtab)
{
	int	exit_code;
	int	backup_stdin;
	int	backup_stdout;

	if (st_get_tools(tools, input) == EXIT_FAILURE || tools->input_fd < 0 || \
	tools->output_fd < 0)
		return (EXIT_FAILURE);
	backup_stdin = dup(STDIN_FILENO);
	backup_stdout = dup(STDOUT_FILENO);
	dup2(tools->input_fd, STDIN_FILENO);
	dup2(tools->output_fd, STDOUT_FILENO);
	exit_code = execute_builtin(tools->cmd_args, symtab, PARENT);
	if (tools->input_fd > STDERR_FILENO)
		close(tools->input_fd);
	if (tools->output_fd > STDERR_FILENO)
		close(tools->output_fd);
	dup2(backup_stdin, STDIN_FILENO);
	dup2(backup_stdout, STDOUT_FILENO);
	close(backup_stdin);
	close(backup_stdout);
	return (exit_code);
}

/* 
	The function ensures that the parent process wait for the child processes 
	to finish and captures the exit code of the last pipe, which it returns.
 */
int	ex_wait_for_last_child(int id, size_t total_cmds)
{
	int	wait_status;
	int	exit_code;

	waitpid(id, &wait_status, 0);
	if (WIFEXITED(wait_status))
		exit_code = WEXITSTATUS(wait_status);
	else if (WIFSIGNALED(wait_status))
		exit_code = WTERMSIG(wait_status) + 128;
	else
		exit_code = 1;
	while (total_cmds > 1)
	{
		wait(&wait_status);
		total_cmds--;
	}
	return (exit_code);
}
