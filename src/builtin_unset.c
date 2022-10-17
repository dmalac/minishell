/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_unset.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/13 17:44:33 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/09/13 17:57:11 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "symtab.h"
#include "builtin.h"
#include "libft.h"

/* 
	this function removes a node from the symbol table corresponding to the 
	variable names provided as arguments.
 */
int	bi_unset(char **args, t_symtab **symtab)
{
	size_t		i;
	int			exit_code;

	i = 1;
	exit_code = EXIT_SUCCESS;
	while (args && args[i])
	{
		if (is_valid_var_name("unset", args[i]) == FALSE)
			exit_code = EXIT_FAILURE;
		else
			*symtab = symtab_remove_node(symtab, args[i]);
		i++;
	}
	return (exit_code);
}
