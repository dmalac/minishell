/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_export_lst.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/14 18:03:18 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/10/17 10:48:14 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

// #include <stdio.h>

#include <errno.h>
#include <string.h>
#include "builtin.h"
#include <unistd.h>
#include "libft.h"
#include "symtab.h"

/* 
	The function sorts a 2D array containing the pointers to all nodes in the 
	symbol table.
 */
static void	st_sort_export_list(t_symtab **export_list)
{
	size_t		i;
	size_t		j;
	t_symtab	*tmp;

	i = 0;
	while (export_list[i])
	{
		j = i + 1;
		while (export_list[j])
		{
			if (ft_strncmp(export_list[i]->key, export_list[j]->key, 40) > 0)
			{
				tmp = export_list[i];
				export_list[i] = export_list[j];
				export_list[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

/* 
	The function displays the alphabetically sorted list of variables from the 
	symbol table. It returns 0 upon success and 1 if a writing error occurs.
 */
static int	st_print_export_list(t_symtab **export_list)
{
	size_t	i;

	i = 0;
	while (export_list[i])
	{
		if (export_list[i]->value)
		{
			if (ft_putstr_fd("declare -x ", STDOUT_FILENO) < 0 || \
			ft_putstr_fd(export_list[i]->key, STDOUT_FILENO) < 0 || \
			ft_putstr_fd("=", STDOUT_FILENO) < 0 || \
			ft_putendl_fd(export_list[i]->value, STDOUT_FILENO) < 0)
				return (builtin_error("export", NULL, "Writing error"), 1);
		}
		else
		{
			if (ft_putstr_fd("declare -x ", STDOUT_FILENO) < 0 || \
			ft_putendl_fd(export_list[i]->key, STDOUT_FILENO) < 0)
				return (builtin_error("export", NULL, "Writing error"), 1);
		}
		i++;
	}
	return (0);
}

/* 
	The function creates a 2D array containing the pointers to all nodes in the 
	symbol table and returns it.
 */
static t_symtab	**st_create_export_list(t_symtab *symtab, size_t count)
{
	t_symtab	**export_list;
	t_symtab	*node;
	size_t		i;

	export_list = malloc(sizeof(t_symtab *) * (count + 1));
	if (!export_list)
		return (ft_putendl_fd(strerror(errno), STDERR_FILENO), NULL);
	i = 0;
	node = symtab;
	while (i < count)
	{
		export_list[i] = node;
		node = node->next;
		i++;
	}
	export_list[i] = NULL;
	return (export_list);
}

/* 
	This function displays a list of all variables contained in the symbol table 
	linked list sorted alphabetically.
 */
int	bi_display_export_list(t_symtab *symtab)
{
	t_symtab	**export_list;
	size_t		count;
	int			exit_code;

	count = symtab_count_nodes(symtab);
	export_list = st_create_export_list(symtab, count);
	if (!export_list)
	{
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	st_sort_export_list(export_list);
	exit_code = st_print_export_list(export_list);
	free(export_list);
	return (exit_code);
}
