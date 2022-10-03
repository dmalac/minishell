/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   executor.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/08 14:27:06 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/09/26 15:53:50 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
// #include "libft.h"	is in minishell.h
#include "executor.h"
// #include "symtab.h"	is in minishell.h
#include "builtin.h"
#include <errno.h>
// #include <stdio.h>	is in minishell.h
// #include <unistd.h>	is in minishell.h

static t_token_lst	*st_goto_nxt_cmd(t_token_lst *node)
{
	while (node && node->token_type != PIPE)
		node = node->next;
	if (node)
		node = node->next;
	return (node);
}

int	st_prepare_to_exit(t_cmd_tools *tools, t_token_lst *input, \
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

int	executor(t_token_lst *input, t_symtab *symtab)
{
	t_token_lst	*node;
	t_cmd_tools	*tools;
	int			pipe_end[2][2];
	int			exit_code;

	tools = tools_init(input, symtab);
	if (!tools)
		return (1);
	node = input;
	exit_code = 0;
	while (exit_code == 0 && tools->cmd <= tools->total_cmds)
	{
		if (tools->id > 0)
			exit_code = pipe_and_fork(&tools->id, tools, pipe_end);
		if (tools->id == 0)
			perform_cmd(tools, node, pipe_end, symtab);
		if ((tools->id > 0) & (tools->total_cmds > 1))
			close_unnecessary_pipes(tools, pipe_end);
		tools->cmd++;
		node = st_goto_nxt_cmd(node);
	}
	exit_code = st_prepare_to_exit(tools, input, symtab, exit_code);
	cleanup(tools);
	return (exit_code);
}
