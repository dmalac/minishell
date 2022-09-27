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

/* swaps values of two variables (useful for builtin function cd) */
static void	st_symtab_swap_value(t_symtab *node1, t_symtab *node2)
{
	char	*tmp;

	tmp = node1->value;
	node1->value = node2->value;
	node2->value = tmp;
}

/* updates the PWD and OLDPWD variables */
static void	st_symtab_update_pwd(t_symtab *oldpwd, t_symtab *pwd)
{
	char	*new_address;

	new_address = getcwd(NULL, 1);
	free(oldpwd->value);
	oldpwd->value = pwd->value;
	pwd->value = new_address;
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
			return (builtin_error("cd", NULL, "OLDPWD not set"), 1);
		else if (chdir(oldpwd->value) < 0) // non-existing address
			return (builtin_error("cd", oldpwd->value, \
			"No such file or directory"), 1);
		printf("%s\n", oldpwd->value);
		st_symtab_swap_value(pwd, oldpwd);
	}
	else
	{
		if (chdir(address) < 0)
			return (builtin_error("cd", address, "No such file or directory"), \
			1);
		st_symtab_update_pwd(oldpwd, pwd);
	}
	return (0);
}
