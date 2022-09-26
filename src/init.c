/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/05 17:26:48 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/09/16 15:46:14 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "minishell.h"
#include "executor.h"
#include "builtin.h"
#include "symtab.h"
#include "libft.h"

t_symtab	*init_symbol_table(void)
{
	extern char	**environ;
	t_symtab	*top;
	t_symtab	*new;
	size_t		i;

	i = 0;
	top = NULL;
	while (environ[i])
	{
		new = symtab_new(environ[i++]);
		if (!new)
		{
			symtab_erase_and_free(&top);
			ft_putendl_fd(strerror(errno), 2);
			return (NULL);
		}
		symtab_add_back(&top, new);
	}
	// DO THIS ONLY IF NECESSARY!!
	new = symtab_new("OLDPWD");
	symtab_add_back(&top, new);
	return (top);
}

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
		input->token_type == SMLR_TH || input->token_type == DSML_TH)
			input = input->next;
		input = input->next;
	}	
	return (0);
}

t_cmd_tools	*tools_init(t_token_lst *input, t_symtab *symtab)
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
	tools->process_tokens[DSML_TH] = process_input_redir2;
	tools->process_tokens[EMPTY] = process_word;
	return (tools);
}
