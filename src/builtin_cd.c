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

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "builtin.h"
#include "libft.h"
#include "symtab.h"

/* 
	This function swaps values of PWD and OLDPWD to reflect the change of 
	working directory. If the PWD variable is not in the symbol	table, the 
	function has it added. It returns 1 if an error occured or 0 upon successful 
	completion.
*/
static int	st_symtab_swap_value(t_symtab *symtab, t_symtab *pwd, \
t_symtab *oldpwd)
{
	char	*tmp;

	if (!pwd)
		pwd = symtab_add_node(&symtab, "PWD");
	if (!pwd)
		return (EXIT_FAILURE);
	tmp = pwd->value;
	pwd->value = oldpwd->value;
	oldpwd->value = tmp;
	return (EXIT_SUCCESS);
}

/* 
	This function updates the PWD and OLDPWD variables to reflect the change of 
	working directory. If the PWD or the OLDPWD variable are not in the symbol 
	table, the function haves them added. It returns 1 if an error occured or 
	0 upon successful completion.
*/
static int	st_symtab_update_pwd(t_symtab *symtab, t_symtab *oldpwd, \
t_symtab *pwd)
{
	char	*new_address;

	new_address = getcwd(NULL, 1);
	if (!oldpwd)
		oldpwd = symtab_add_node(&symtab, "OLDPWD");
	else
		free(oldpwd->value);
	if (!pwd)
		pwd = symtab_add_node(&symtab, "PWD");
	if (!pwd || !oldpwd)
		return (EXIT_FAILURE);
	oldpwd->value = pwd->value;
	pwd->value = new_address;
	return (EXIT_SUCCESS);
}

/* 
	This function attempts to change directory to the address saved in the 
	OLDPWD variable. It returns 0 upon success and 1 if an error occurred.
 */
static int	st_cd_oldpwd(t_symtab *pwd, t_symtab *oldpwd, t_symtab *symtab)
{
	if (!oldpwd || !oldpwd->value)
	{
		builtin_error("cd", NULL, "OLDPWD not set");
		return (EXIT_FAILURE);
	}
	else if (chdir(oldpwd->value) < 0)
	{
		builtin_error("cd", oldpwd->value, strerror(errno));
		return (EXIT_FAILURE);
	}
	ft_putendl_fd(oldpwd->value, STDOUT_FILENO);
	return (st_symtab_swap_value(symtab, pwd, oldpwd));
}

/* 
	This function handles the change of directory to the same directory and 
	updates the OLDPWD variable.
 */
static int	st_cd_same_dir(t_symtab *pwd, t_symtab *oldpwd)
{
	if (chdir(pwd->value) < 0)
		return (builtin_error("cd", "getcwd", strerror(errno)), EXIT_FAILURE);
	symtab_update_value(oldpwd, ft_strdup(pwd->value));
	return (EXIT_SUCCESS);
}

/* 
	This function changes the directory to the address provided as argument and 
	updates the values of the PWD and OLDPWD variables. 
	If the address is '-', the address stored in the OLDPWD variable is used.
	It returns 1 if an error occured or 0 upon successful completion.
 */
int	bi_cd(char *address, t_symtab *symtab)
{
	t_symtab	*pwd;
	t_symtab	*oldpwd;

	if (!address)
		return (EXIT_FAILURE);
	pwd = symtab_get_node(symtab, "PWD");
	oldpwd = symtab_get_node(symtab, "OLDPWD");
	if (address && ft_strncmp(address, "-", 2) == 0)
		return (st_cd_oldpwd(pwd, oldpwd, symtab));
	if (address && ft_strncmp(address, ".", 2) == 0 && pwd && pwd->value)
		return (st_cd_same_dir(pwd, oldpwd));
	else
	{
		if (chdir(address) < 0)
			return (builtin_error("cd", address, strerror(errno)), \
			EXIT_FAILURE);
		return (st_symtab_update_pwd(symtab, oldpwd, pwd));
	}
}
