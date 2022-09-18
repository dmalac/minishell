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

int	bi_unset(char **args, t_symtab *symtab)
{
	size_t		i;

	i = 1;
	while (args[i])
	{
		symtab_remove_node(&symtab, args[i]);
		i++;
	}
	return (0);
}
