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

int	bi_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 1);
	if (!pwd)
		return (errno);
	printf("%s\n", pwd);	// or ft_putendl?
	return (0);
}
