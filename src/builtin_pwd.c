/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_pwd.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/14 18:17:37 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/09/16 13:28:30 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include "symtab.h"

// int	bi_pwd(t_symtab *symtab)
int	bi_pwd(void)
{
	char	*pwd;
	// char	*to_be_freed;

	// to_be_freed = NULL;
	// pwd = symtab_get_value(symtab, "PWD");
	// if (!pwd)
	// {
	pwd = getcwd(NULL, 1);
	if (!pwd)
		return (errno);
		// else
			// to_be_freed = pwd;
	// }
	printf("%s\n", pwd);
	return (0);
}
