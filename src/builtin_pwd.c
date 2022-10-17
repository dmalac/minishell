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

#include <unistd.h>
#include "builtin.h"
#include "libft.h"

/* 
	This function returns the address of the current working dictionary. It 
	returns 1 if an error occured or 0 upon successful completion.
 */
int	bi_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		builtin_error("pwd", NULL, \
		"Permission denied");
		return (EXIT_FAILURE);
	}
	ft_putendl_fd(pwd, STDOUT_FILENO);
	free(pwd);
	return (EXIT_SUCCESS);
}
