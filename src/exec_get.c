/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_get.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/19 13:11:08 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/10/06 16:28:32 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "symtab.h"
#include <stdlib.h>
#include "libft.h"
#include "executor.h"
#include <errno.h>
#include <string.h>

/* 
	The function returns the number of commands in the input
 */
static size_t	st_count_args(t_token_lst *input)
{
	size_t	words;

	words = 0;
	while (input && input->token_type != PIPE)
	{
		if (input->token_type == WORD || input->token_type == EMPTY)
			words++;
		else if (input->token_type == GRT_TH || input->token_type == DGRT_TH || \
		input->token_type == SMLR_TH || input->token_type == DSML_TH)
			input = input->next;
		input = input->next;
	}
	return (words);
}

/* 
	The function allocates memory for a 2D char array to store the arguments 
	belonging to a particular command in the input. It returns 1 if an error 
	occured when allocating memory or 0 upon successful completion.
 */
int	get_args(t_cmd_tools *tools, t_token_lst *input)
{
	size_t	no_of_args;
	size_t	i;

	no_of_args = st_count_args(input);
	if (no_of_args > 0)
	{
		tools->cmd_args = malloc(sizeof(char *) * (no_of_args + 1));
		if (!tools->cmd_args)
			return (ft_putendl_fd(strerror(errno), STDERR_FILENO), EXIT_FAILURE);
		i = 0;
		while (i <= no_of_args)
			tools->cmd_args[i++] = NULL;
	}
	else
		tools->cmd_args = NULL;
	return (EXIT_SUCCESS);
}

/* 
	The function adds a '/' character at the end of each path in the struct 
	tools.paths. It returns 1 if an error occured or 0 upon successful 
	completion.
 */
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
			return (EXIT_FAILURE);
		i++;
	}
	return (EXIT_SUCCESS);
}

/* 
	The function extracts the paths from the variable PATH contained in the 
	symbol table and stores them in the struct tools.
	The function returns 1 if an error occured or 0 upon successful completion.
 */
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
				return (EXIT_SUCCESS);
		}
		if (!tools->paths)
		{
			ft_putendl_fd(strerror(errno), STDERR_FILENO);
			return (EXIT_FAILURE);
		}
	}
	else
		tools->paths = NULL;
	return (EXIT_SUCCESS);
}

/* 
	This function iterates over the symbol table and saves the variables and 
	their values in a 2D char array in the struct tools. The format is 
	<variable_name>=<value>. Only variables with non-null values are included in 
	the array. 
	The function returns 1 if an error occured or 0 upon successful completion.
 */
int	get_env_var(t_symtab *symtab, t_cmd_tools *tools)
{
	size_t		i;
	t_symtab	*node;
	size_t		total_nodes;

	total_nodes = symtab_count_nodes(symtab);
	tools->env_var = malloc(sizeof(char *) * (total_nodes + 1));
	if (!tools->env_var)
		return (ft_putendl_fd(strerror(errno), STDERR_FILENO), EXIT_FAILURE);
	node = symtab;
	i = 0;
	while (node)
	{
		if (node->value)
		{
			tools->env_var[i] = ft_strjoin_env(node->key, node->value, '=');
			if (!tools->env_var[i])
				return (EXIT_FAILURE);
			i++;
		}
		node = node->next;
	}
	tools->env_var[i] = NULL;
	return (EXIT_SUCCESS);
}
