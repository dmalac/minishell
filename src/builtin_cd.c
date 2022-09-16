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

static char	*st_get_address(char **args, t_symtab *symtab)
{
	char	*address;

	if (!args[1])
		address = symtab_get_value(symtab, "HOME");
	else
		address = args[1];
	return (address);
}

/* char **args to be replaced by char* address (from t_token_lst->content) */
int	bi_cd(char **args, t_symtab *symtab)
{
	char		*address;
	t_symtab	*pwd;
	t_symtab	*oldpwd;

	if (!args)
		return (1);
	pwd = symtab_get_node(symtab, "PWD");
	oldpwd = st_get_old_pwd(symtab);
	address = st_get_address(args, symtab);
	if (address && ft_strncmp(address, "-", 2) == 0)
	{
		if (!oldpwd->value)
		{}
			//bash: cd: OLDPWD not set
		else if (chdir(oldpwd->value) < 0) // non-existing address
			return (perror("ERROR MESSAGES TO BE EDITED: chdir"), 1);
			// use errno!
			// bash: cd: some/nonsense/address: No such file or directory
		printf("%s\n", oldpwd->value);
		symtab_swap_value(pwd, oldpwd);
	}
	else
	{
		if (chdir(address) < 0)
			return (perror("ERROR MESSAGES TO BE EDITED: chdir"), 1);
			// use errno!
		symtab_update_pwd(oldpwd, pwd);
	}
	return (0);
}
