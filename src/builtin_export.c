/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_export.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/14 18:03:18 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/09/27 16:18:10 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <errno.h>
#include "symtab.h"
#include "libft.h"
#include "builtin.h"

/* To do: finalise error handling & exit codes */

static int	st_perform_export(char **args, char **var_name, char **var_val, \
t_symtab *symtab)
{
	size_t	i;
	int		exit_code;

	i = 1;
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
	return (EXIT_SUCCESS);
}

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
