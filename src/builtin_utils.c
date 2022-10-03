/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_utils.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/14 12:35:01 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/10/03 14:46:22 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "symtab.h"
#include "builtin.h"
#include "libft.h"
#include <stdio.h>
#include <string.h>

void	builtin_error(char *bi_name, char *error_cause, char *error_msg)
{
	ft_putstr_fd("bash: ", 2);
	ft_putstr_fd(bi_name, 2);
	ft_putstr_fd(": ", 2);
	if (error_cause)
	{
		ft_putstr_fd("`", 2);	// NOT when cd
		ft_putstr_fd(error_cause, 2);
		ft_putstr_fd("': ", 2);
	}
	ft_putendl_fd(error_msg, 2);
}

int	is_valid_var_name(char *bi_name, char *identifier)
{
	size_t	i;

	i = 0;
	if (!(identifier[i] == '_' || ft_isalpha(identifier[i])))
	{
		builtin_error(bi_name, identifier, "not a valid identifier");
		return (FALSE);
	}
	i++;
	while (identifier[i])
	{
		if (ft_isalnum(identifier[i]) == 0 && identifier[i] != '_')
		{
			builtin_error(bi_name, identifier, "not a valid identifier");
			return (FALSE);
		}
		i++;
	}
	return (TRUE);
}

int	is_builtin(char *cmd)
{
	if (ft_strncmp(cmd, "echo", 5) == 0 || ft_strncmp(cmd, "cd", 3) == 0 || \
	ft_strncmp(cmd, "pwd", 4) == 0 || ft_strncmp(cmd, "export", 7) == 0 || \
	ft_strncmp(cmd, "unset", 6) == 0 || ft_strncmp(cmd, "exit", 5) == 0 || \
	ft_strncmp(cmd, "env", 4) == 0)
		return (TRUE);
	else
		return (FALSE);
}

int	execute_builtin(char **args, t_symtab *symtab, int who)
{
	if (args[0][0] == 'c')
		return (bi_cd(args[1], symtab));
	else if (args[0][0] == 'p')
		return (bi_pwd());
	else if (args[0][0] == 'u')
		return (bi_unset(args, symtab));
	else if (args[0][0] == 'e' && args[0][1] == 'c')
		return (bi_echo(args));
	else if (args[0][0] == 'e' && args[0][1] == 'n')
		return (bi_env(symtab));
	else if (args[0][0] == 'e' && args[0][1] == 'x' && args[0][2] == 'p')
		return (bi_export(args, symtab));
	else if (args[0][0] == 'e' && args[0][1] == 'x' && args[0][2] == 'i')
		return (bi_exit(args, who));
	return (0);
}
