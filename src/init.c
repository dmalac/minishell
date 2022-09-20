/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/05 17:26:48 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/09/16 15:46:14 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include "symtab.h"
#include "libft.h"

t_symtab	*init_symbol_table(void)
{
	extern char	**environ;
	t_symtab	*top;
	t_symtab	*new;
	size_t		i;

	i = 0;
	top = NULL;
	while (environ[i])
	{
		new = symtab_new(environ[i++]);
		if (!new)
			printf("Malloc of new symtab node didn't go well\n");
			// free_and_exit(top);
		symtab_add_back(&top, new);
	}
	new = symtab_new("OLDPWD");
	symtab_add_back(&top, new);
	return (top);
}
