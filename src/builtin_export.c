/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_export.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/14 18:03:18 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/10/04 13:16:46 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <errno.h>
#include "symtab.h"
#include "libft.h"
#include "builtin.h"

/* 
	The function iterates over the array of arguments and ensures that arguments 
	containing valid variable names are further processed.	
 */
static int	st_perform_export(char **args, char **var_name, char **var_val, \
t_symtab *symtab)
{
	size_t	i;
	int		exit_code;

	i = 1;
	exit_code = EXIT_SUCCESS;
	while (args[i])
	{
		if (get_var_name(args[i], var_name) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		else if (is_valid_var_name("export", *var_name) == FALSE)
		{
			free(*var_name);
			exit_code = EXIT_FAILURE;
		}
		else if (export_variable(args[i], var_name, var_val, symtab) == \
		EXIT_FAILURE)
			return (EXIT_FAILURE);
		i++;
	}
	return (exit_code);
}

/* 
	This function assesses whether any arguments have been provided for the 
	builtin function export. If there is at least one argument, another 
	function is called that will add new variables or change existing variables 
	value, as necessary. If no arguments are provided, a function is called that 
	prints a list of the variables contained in the symbol table in the correct 
	format.
 */
int	bi_export(char **args, t_symtab *symtab)
{
	char		*var_name;
	char		*var_val;
	int			exit_code;

	exit_code = 0;
	if (args && !args[1])
		exit_code = display_export_list(symtab);
	else if (args)
		exit_code = st_perform_export(args, &var_name, &var_val, symtab);
	return (exit_code);
}
