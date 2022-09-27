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
#include <string.h>
#include <stdlib.h>
#include "builtin.h"

int	bi_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		builtin_error("pwd", NULL, \
		"Failed to retrieve the working directory address");
		return (1);
	}
	printf("%s\n", pwd);
	free(pwd);
	return (0);
}
