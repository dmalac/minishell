/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_export_var.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/14 18:03:18 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/09/26 18:35:11 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <errno.h>
#include "symtab.h"
#include "libft.h"

/* To do: finalise error handling & exit codes */

int	is_valid_var_name(char *var_name)
{
	size_t	i;

	i = 0;
	if (!(var_name[i] == '_' || ft_isalpha(var_name[i])))
	{
		printf("export: `%s\': not a valid identifier\n", var_name);
		return (0);
	}
	i++;
	while (var_name[i])
	{
		if (ft_isalnum(var_name[i]) == 0 && var_name[i] != '_')
		{
			printf("export: `%s\': not a valid identifier\n", var_name);
			return (0);
		}
		i++;
	}
	return (1);
}

char	*get_var_name(char *arg)
{
	size_t	len_var_name;
	char	*var_name;

	len_var_name = 0;
	while (arg[len_var_name] && arg[len_var_name] != '=')
		len_var_name++;
	var_name = ft_substr(arg, 0, len_var_name);
	return (var_name);
}

char	*get_var_value(char *arg)
{
	char	*var_value;
	size_t	len_var_name;

	len_var_name = 0;
	while (arg[len_var_name] && arg[len_var_name] != '=')
		len_var_name++;
	if (arg[len_var_name] == '=')
		var_value = ft_substr(arg, len_var_name + 1, \
		ft_strlen(arg) - len_var_name - 1);
		// if (!var_value -> malloc error?? how does it differ from no '='?)
	else
		var_value = NULL;
	return (var_value);
}
