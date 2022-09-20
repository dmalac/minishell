/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/09 17:09:40 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/09/16 16:06:04 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "symtab.h"
#include <stdlib.h>
#include "libft.h"
#include "executor.h"
#include <errno.h>

static size_t	st_count_args(t_token_lst *input)
{
	size_t	words;

	words = 0;
	while (input && input->token_type != PIPE)
	{
		if (input->token_type == WORD)
			words++;
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

char	**get_paths(t_symtab *symtab)
{
	char	**paths;
	char	*all_paths;
	char	*temp;
	size_t	i;

	paths = NULL;
	all_paths = symtab_get_value(symtab, "PATH");
	if (all_paths)
	{
		paths = ft_split(all_paths, ':');
		if (paths)
		{
			i = -1;
			while (paths[++i])
			{
				temp = paths[i];
				paths[i] = ft_strjoin(paths[i], "/");
				if (!paths[i])
					return (free_array(paths), NULL);
				free (temp);
			}
		}
	}
	return (paths);
}

char	**get_env_var(t_symtab *symtab)
{
	size_t		i;
	t_symtab	*node;
	char		**env_var;

	i = 0;
	env_var = NULL;
	node = symtab;
	while (node)
	{
		if (node->value)
			i++;
		node = node->next;
	}
	env_var = malloc(sizeof(char *) * (i + 1));
	if (!env_var)
		return (NULL);
	node = symtab;
	i = 0;
	while (node)
	{
		if (node->value)
		{
			env_var[i] = ft_strjoin_env(node->key, node->value, '=');
			if (!env_var[i])
				return (free_array(env_var), NULL);
			i++;
			node = node->next;
		}
	}
	env_var[i] = NULL;
	return (env_var);
}
