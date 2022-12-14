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
#include <errno.h>
#include <string.h>
#include "executor.h"
#include "builtin.h"
#include "symtab.h"
#include "libft.h"

/* 
	This function updates the value of the SHLVL variable.
 */
static void	st_update_shlvl(t_symtab *symtab)
{
	t_symtab	*node;
	int			no;

	node = symtab_get_node(symtab, "SHLVL");
	no = ft_atoi(node->value);
	free(node->value);
	node->value = ft_itoa(no + 1);
}

/* 
	The function initialises the symbol table and copies all environment 
	variables into it.
 */
t_symtab	*init_symbol_table(void)
{
	extern char	**environ;
	t_symtab	*top;
	t_symtab	*new;
	t_symtab	*tmp;
	size_t		i;

	i = 0;
	top = NULL;
	while (environ[i])
	{
		new = symtab_new(environ[i++]);
		if (!new)
		{
			symtab_erase_and_free(&top);
			return (ft_putendl_fd(strerror(errno), STDERR_FILENO), NULL);
		}
		symtab_add_back(&top, new);
	}
	tmp = symtab_get_node(top, "PS1");
	if (!tmp)
		new = symtab_add_node(&top, "PS1=minishell> ");
	else
		symtab_update_value(tmp, ft_strdup("minishell> "));
	st_update_shlvl(top);
	return (top);
}

/* 
	This function counts the commands in the input linked list and returns the 
	result.
 */
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

/* 
	The function checks whether the input only contains one command, which is 
	also a builtin function.
 */
static int	st_check_if_only_builtin(t_token_lst *input, t_cmd_tools *tools)
{
	if (tools->total_cmds > 1)
		return (FALSE);
	while (input && input->token_type != PIPE)
	{
		if (input->token_type == WORD)
			return (is_builtin(input->content));
		else if (input->token_type == GRT_TH || input->token_type == DGRT_TH || \
		input->token_type == SMLR_TH || input->token_type == DSML_TH)
			input = input->next;
		input = input->next;
	}	
	return (FALSE);
}

/* 
	This function initialises the struct tools to be used by the execution 
	process.
 */
t_cmd_tools	*ex_tools_init(t_token_lst *input, t_symtab *symtab)
{
	t_cmd_tools	*tools;

	tools = malloc(sizeof(t_cmd_tools));
	if (!tools)
		return (ft_putendl_fd(strerror(errno), STDERR_FILENO), NULL);
	tools->id = 1;
	tools->cmd = 1;
	tools->total_cmds = st_count_cmds(input);
	tools->input_fd = STDIN_FILENO;
	tools->output_fd = STDOUT_FILENO;
	tools->cmd_args = NULL;
	if (ex_get_paths(symtab, tools) == EXIT_FAILURE || \
	ex_get_env_var(symtab, tools) == EXIT_FAILURE)
		return (ex_cleanup_tools(&tools), NULL);
	if (check_heredoc(input, tools) == EXIT_FAILURE || \
	get_heredoc(tools->heredoc, symtab) == EXIT_FAILURE)
		return (ex_cleanup_tools(&tools), NULL);
	tools->builtin_only = st_check_if_only_builtin(input, tools);
	tools->process_tokens[WORD] = ex_process_word;
	tools->process_tokens[GRT_TH] = ex_process_output_redir1;
	tools->process_tokens[SMLR_TH] = ex_process_input_redir1;
	tools->process_tokens[DGRT_TH] = ex_process_output_redir2;
	tools->process_tokens[DSML_TH] = ex_process_input_redir2;
	tools->process_tokens[EMPTY] = ex_process_word;
	return (tools);
}
