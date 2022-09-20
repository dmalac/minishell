/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_get.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/19 13:11:08 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/09/20 17:50:03 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "symtab.h"
#include <stdlib.h>
#include "libft.h"
#include "executor.h"
#include <errno.h>
#include <string.h>

static size_t	st_count_args(t_token_lst *input)
{
	size_t	words;

	words = 0;
	while (input && input->token_type != PIPE)
	{
		if (input->token_type == WORD)
			words++;
		else if (input->token_type == GRT_TH || input->token_type == DGRT_TH || \
		input->token_type == SMLR_TH || input->token_type == DSMLR_TH)
			input = input->next;
		input = input->next;
	}
	return (words);
}

void	get_args(t_cmd_tools *tools, t_token_lst *input)
{
	size_t	no_of_args;
	size_t	i;

	no_of_args = st_count_args(input);
	if (no_of_args > 0)
	{
		tools->cmd_args = malloc(sizeof(char *) * (no_of_args + 1));
		if (!tools->cmd_args)
			child_error_and_exit(errno, tools, NULL);
		i = 0;
		while (i <= no_of_args)
			tools->cmd_args[i++] = NULL;
	}
	else
		tools->cmd_args = NULL;
}

static int	st_add_slash_to_paths(t_cmd_tools *tools)
{
	char	*temp;
	size_t	i;

	i = 0;
	while (tools->paths[i])
	{
		temp = tools->paths[i];
		tools->paths[i] = ft_strjoin(tools->paths[i], "/");
		free (temp);
		if (!tools->paths[i])
			return (1);
		i++;
	}
	return (0);
}

int	get_paths(t_symtab *symtab, t_cmd_tools *tools)
{
	char	*all_paths;

	all_paths = symtab_get_value(symtab, "PATH");
	if (all_paths)
	{
		tools->paths = ft_split(all_paths, ':');
		if (tools->paths)
		{
			if (st_add_slash_to_paths(tools) == 0)
				return (0);
		}
		if (!tools->paths)
		{
			strerror(errno);
			return (1);
		}
	}
	else
		tools->paths = NULL;
	return (0);
}

int	get_env_var(t_symtab *symtab, t_cmd_tools *tools)
{
	size_t		i;
	t_symtab	*node;
	size_t		total_nodes;

	total_nodes = symtab_count_nodes(symtab);
	tools->env_var = malloc(sizeof(char *) * (total_nodes + 1));
	if (!tools->env_var)
		return (1);
	node = symtab;
	i = 0;
	while (node)
	{
		if (node->value)
		{
			tools->env_var[i] = ft_strjoin_env(node->key, node->value, '=');
			if (!tools->env_var[i])
				return (1);
			i++;
		}
		node = node->next;
	}
	tools->env_var[i] = NULL;
	return (0);
}
