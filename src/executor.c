/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   executor.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/08 14:27:06 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/09/16 15:53:16 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "libft.h"
#include "executor.h"
#include "symtab.h"
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

size_t	st_count_cmds(t_token_lst *input)
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

t_cmd_tools	*st_tools_init(t_token_lst *input, t_symtab *symtab)
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
	tools->paths = get_paths(symtab);
	tools->env_var = get_env_var(symtab);
	if (!tools->env_var)
		return (free_array(tools->paths), NULL);
	tools->heredoc = check_heredoc(input);
	if (tools->heredoc)
		get_next_heredoc(tools->heredoc);
	tools->process_tokens[WORD] = process_word;
	tools->process_tokens[GRT_TH] = process_output_redir1;
	tools->process_tokens[SMLR_TH] = process_input_redir1;
	tools->process_tokens[DGRT_TH] = process_output_redir2;
	tools->process_tokens[DSMLR_TH] = process_input_redir2;
	tools->process_tokens[EMPTY] = process_word;
	return (tools);
}

t_token_lst	*st_goto_nxt_cmd(t_token_lst *node)
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
		return (1);	// or sth else?
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
	if (exit_code != 0)
		return (print_error_message(exit_code, NULL), exit_code); // should it wait here?
	else if (tools->id > 0)
		exit_code = wait_for_last_child(tools->id, tools->total_cmds);
	cleanup(tools);
	return (exit_code);
}
