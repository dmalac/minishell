/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   executor.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/08 14:27:06 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/09/20 18:32:16 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "libft.h"
#include "executor.h"
#include "symtab.h"
#include "builtin.h"
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

static size_t	st_count_cmds(t_token_lst *input)
{
	size_t	count;

	count = 0;
	while (input)
	{
		if (input->token_type == PIPE)
			count++;
		input = input->next;
	}
	return (count + 1);
}

static int	st_check_if_cmd_builtin(t_token_lst *input, t_cmd_tools *tools)
{
	if (tools->total_cmds > 1)
		return (0);
	while (input && input->token_type != PIPE)
	{
		if (input->token_type == WORD)
		{
			if (is_builtin(input->content))
				return (1);
			else
				return (0);
		}
		else if (input->token_type == GRT_TH || input->token_type == DGRT_TH || \
		input->token_type == SMLR_TH || input->token_type == DSMLR_TH)
			input = input->next;
		input = input->next;
	}	
	return (0);
}

static t_cmd_tools	*st_tools_init(t_token_lst *input, t_symtab *symtab)
{
	t_cmd_tools	*tools;

	tools = malloc(sizeof(t_cmd_tools));
	if (!tools)
		return (NULL);
	tools->id = 1;
	tools->cmd = 1;
	tools->total_cmds = st_count_cmds(input);
	tools->input_fd = STDIN_FILENO;
	tools->output_fd = STDOUT_FILENO;
	tools->cmd_args = NULL;
	if (get_paths(symtab, tools) == 1 || get_env_var(symtab, tools) == 1)
		return (cleanup(tools), NULL);
	if (check_heredoc(input, tools) == 1 || get_heredoc(tools->heredoc) == 1)
		return (cleanup(tools), NULL);
	tools->builtin_only = st_check_if_cmd_builtin(input, tools);
	tools->process_tokens[WORD] = process_word;
	tools->process_tokens[GRT_TH] = process_output_redir1;
	tools->process_tokens[SMLR_TH] = process_input_redir1;
	tools->process_tokens[DGRT_TH] = process_output_redir2;
	tools->process_tokens[DSMLR_TH] = process_input_redir2;
	tools->process_tokens[EMPTY] = process_word;
	return (tools);
}

static t_token_lst	*st_goto_nxt_cmd(t_token_lst *node)
{
	while (node && node->token_type != PIPE)
		node = node->next;
	if (node)
		node = node->next;
	return (node);
}

int	executor(t_token_lst *input, t_symtab *symtab)
{
	t_token_lst	*node;
	t_cmd_tools	*tools;
	int			pipe_end[2][2];
	int			exit_code;

	tools = st_tools_init(input, symtab);
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
		if (tools->id > 0 & tools->total_cmds > 1)
			close_unnecessary_pipes(tools, pipe_end);
		tools->cmd++;
		node = st_goto_nxt_cmd(node);
	}
	if (exit_code == 0 && tools->id > 0)
		exit_code = wait_for_last_child(tools->id, tools->total_cmds);
	if (exit_code == -1)
		exit_code = parent_exec_builtin(tools, input, symtab);
	cleanup(tools);
	// printf("Executor returning exit code %d\n", exit_code);	// delete
	return (exit_code);
}
