/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_cd.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/14 18:17:06 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/09/15 14:41:44 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include "builtin.h"
#include "symtab.h"
#include "libft.h"

static t_symtab	*st_get_old_pwd(t_symtab *symtab)
{
	t_symtab	*oldpwd;

	oldpwd = symtab_get_node(symtab, "OLDPWD");
	if (!oldpwd)
	{
		oldpwd = symtab_new("OLDPWD");
		symtab_add_back(&symtab, oldpwd);
	}
	return (oldpwd);
}

int	bi_cd(char *address, t_symtab *symtab)
{
	t_symtab	*pwd;
	t_symtab	*oldpwd;

	if (!address)
		return (1);
	pwd = symtab_get_node(symtab, "PWD");
	oldpwd = st_get_old_pwd(symtab);
	if (address && ft_strncmp(address, "-", 2) == 0)
	{
		if (!oldpwd->value)
		{
			ft_putendl_fd("bash: cd: OLDPWD not set", 2);
			return (1);
		}
		else if (chdir(oldpwd->value) < 0) // non-existing address
		{
			ft_putstr_fd("bash: cd: ", 2);
			ft_putstr_fd(oldpwd->value, 2);
			ft_putendl_fd(": No such file or directory", 2);
			return (1);
		}
		printf("%s\n", oldpwd->value);
		symtab_swap_value(pwd, oldpwd);
	}
	else
	{
		if (chdir(address) < 0)
		{
			ft_putstr_fd("bash: cd: ", 2);
			ft_putstr_fd(address, 2);
			ft_putendl_fd(": No such file or directory", 2);
			return (1);
		}
		symtab_update_pwd(oldpwd, pwd);
	}
	return (0);
}
