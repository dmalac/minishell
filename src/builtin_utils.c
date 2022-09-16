/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_utils.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/14 12:35:01 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/09/16 13:30:34 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "symtab.h"
#include "builtin.h"
#include "libft.h"
#include <stdio.h>

int	is_builtin(char *cmd)
{
	if (ft_strncmp(cmd, "echo", 5) == 0 || ft_strncmp(cmd, "cd", 3) == 0 || \
	ft_strncmp(cmd, "pwd", 4) == 0 || ft_strncmp(cmd, "export", 7) == 0 || \
	ft_strncmp(cmd, "unset", 6) == 0 || ft_strncmp(cmd, "exit", 5) == 0 || \
	ft_strncmp(cmd, "env", 4) == 0)
		return (1);
	else
		return (0);
}

int	execute_builtin(char **args, t_symtab *symtab)
{
	if (args[0][0] == 'c')
		return (bi_cd(args, symtab));
	if (args[0][0] == 'p')
		return (bi_pwd());
	if (args[0][0] == 'u')
		return (bi_unset(args, symtab));
	if (args[0][0] == 'e' && args[0][1] == 'c')
		return (bi_echo(args));
	if (args[0][0] == 'e' && args[0][1] == 'n')
		return (bi_env(symtab));
	if (args[0][0] == 'e' && args[0][1] == 'x' && args[0][2] == 'p')
		return (bi_export(args, symtab));
	if (args[0][0] == 'e' && args[0][1] == 'x' && args[0][2] == 'i')
		printf("Here I'd call exit, if it existed...\n");
		// return (bi_exit());
	return (0);	// delete
}