/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_export_var.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/14 18:03:18 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/09/27 16:44:47 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "symtab.h"
#include "libft.h"
#include "builtin.h"

int	get_var_name(char *arg, char **var_name)
{
	size_t	len_var_name;

	len_var_name = 0;
	while (arg[len_var_name] && arg[len_var_name] != '=')
		len_var_name++;
	if (len_var_name == 0)
		*var_name = ft_substr(arg, 0, ft_strlen(arg));
	else
		*var_name = ft_substr(arg, 0, len_var_name);
	if (!(*var_name))
		return (ft_putendl_fd(strerror(errno), 2), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	get_var_val(char *arg, char **var_val)
{
	size_t	len_var_name;

	len_var_name = 0;
	while (arg[len_var_name] && arg[len_var_name] != '=')
		len_var_name++;
	if (arg[len_var_name] == '=')
	{
		*var_val = ft_substr(arg, len_var_name + 1, \
		ft_strlen(arg) - len_var_name - 1);
		if (!(*var_val))
			return (ft_putendl_fd(strerror(errno), 2), EXIT_FAILURE);
	}
	else
		*var_val = NULL;
	return (EXIT_SUCCESS);
}

int	export_variable(char *arg, char **var_name, char **var_val, \
t_symtab *symtab)
{
	t_symtab	*node;

	if (get_var_val(arg, var_val) == EXIT_FAILURE)
	{
		free(*var_name);
		return (EXIT_FAILURE);
	}
	node = symtab_get_node(symtab, *var_name);
	if (!node && symtab_add_var(symtab, *var_name, *var_val) == \
		EXIT_FAILURE)
		return (EXIT_FAILURE);
	else if (node && *var_val)
		symtab_update_value(node, *var_val);
	return (EXIT_SUCCESS);
}
