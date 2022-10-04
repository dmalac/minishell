/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   executor.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/08 14:27:06 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/10/04 18:48:58 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
// #include "libft.h"	is in minishell.h
#include "executor.h"
// #include "symtab.h"	is in minishell.h
#include "builtin.h"
#include <errno.h>
#include <readline/readline.h>
#include <signal.h>
// #include <stdio.h>	is in minishell.h
// #include <unistd.h>	is in minishell.h

/* 
	The function returns the firs node of the following command (i.e., the first
	token following the next PIPE). If there is no next command, it returns NULL.
 */
static t_token_lst	*st_goto_nxt_cmd(t_token_lst *node)
{
	while (node && node->token_type != PIPE)
		node = node->next;
	if (node)
		node = node->next;
	return (node);
}

/* 
	If the process had been forked (-> exit_code >= 0), then a function is 
	called which lets the parent process wait for the child processes to finish 
	and captures the exit code of the last pipe.
	If the process had not been forked because the input only contained one 
	command and it was a builtin function (-> exit_code == -1), the function 
	that executes the builtin function is called and the exit code is captured. 
	The function then returns the exit code.
 */
static int	st_prepare_to_exit(t_cmd_tools *tools, t_token_lst *input, \
t_symtab *symtab, int exit_code)
{
	if (exit_code == -1)
		exit_code = parent_exec_builtin(tools, input, symtab);
	else if (exit_code == 0)
		exit_code = wait_for_last_child(tools->id, tools->total_cmds);
	else if (exit_code == 1)
	{
		while (tools->cmd > 1)
		{
			wait(NULL);
			tools->cmd--;
		}
	}
	return (exit_code);
}

/* TO BE INCLUDED: SIGNAL HANDLING */

/* 
	Executor manages the execution of the commands and builtin functions 
	contained in the user input. It takes care of pipes and redirections, 
	creates required output files and executes the commands either in a child 
	process or in the parent process.
*/

int	executor(t_token_lst *input, t_symtab *symtab)
{
	t_token_lst	*node;
	t_cmd_tools	*tools;
	int			pipe_end[2][2];
	int			exit_code;

	tools = tools_init(input, symtab);
	if (!tools)
		return (EXIT_FAILURE);
	node = input;
	exit_code = EXIT_SUCCESS;
	while (exit_code == 0 && tools->cmd <= tools->total_cmds)
	{
		if (tools->id > 0)
			exit_code = pipe_and_fork(&tools->id, tools, pipe_end);
		if (tools->id == 0)
			perform_cmd(tools, node, pipe_end, symtab);
		if (tools->id > 0 & tools->total_cmds > 1)
			close_unnecessary_pipes(tools, pipe_end);
		tools->cmd++;
		node = st_goto_nxt_cmd(node);
	}
	exit_code = st_prepare_to_exit(tools, input, symtab, exit_code);
	cleanup(tools);
	return (exit_code);
}
