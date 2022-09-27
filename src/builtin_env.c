/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_env.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/14 18:17:18 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/09/14 18:17:20 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "symtab.h"
#include "builtin.h"
#include <stdio.h>

int	bi_env(t_symtab *symtab)
{
	while (symtab)
	{
		if (symtab->value)
		{
			if (printf("%s=%s\n", symtab->key, symtab->value) < 0)
				return (builtin_error("env", NULL, "Writing error"), 1);
		}
		symtab = symtab->next;
	}
	return (0);
}
