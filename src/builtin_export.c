/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_export.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/14 18:03:18 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/09/26 18:41:28 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <errno.h>
#include "symtab.h"
#include "libft.h"
#include "builtin.h"

/* To do: finalise error handling & exit codes */

int	bi_export(char **args, t_symtab *symtab)
{
	size_t		i;
	char		*var_name;
	char		*var_value;
	t_symtab	*node;
	int			exit_code;

	i = 1;
	exit_code = 0;
	// HOW DO I KNOW IT WORKED? EXIT CODES...
	while (args[i])
	{
		var_name = get_var_name(args[i]);
		// if (!var_name -> malloc error)
		// printf("var_name is %s\n", var_name);
		if (is_valid_var_name(var_name) == 0)
			exit_code = 1;
		else
		{
			var_value = get_var_value(args[i]);
			// printf("\tVar value of %s is %s\n", var_name, var_value);
			node = symtab_get_node(symtab, var_name);
			if (!node)
				exit_code = symtab_add_variable(symtab, args[i]);
			else if (node && var_value)
				symtab_update_value(node, var_value);
		}
		i++;
	}
	if (!args[1])
		exit_code = display_export_list(symtab);
	return (exit_code);	// adjust
}
