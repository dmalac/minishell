/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_export.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/14 18:03:18 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/09/15 14:22:13 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <errno.h>
#include "symtab.h"
#include "libft.h"

void	st_sort_export_list(t_symtab **export_list)
{
	size_t		i;
	size_t		j;
	t_symtab	*tmp;

	i = 0;
	while (export_list[i])
	{
		j = i + 1;
		while (export_list[j])
		{
			if (ft_strncmp(export_list[i]->key, export_list[j]->key, 40) > 0)
			{
				tmp = export_list[i];
				export_list[i] = export_list[j];
				export_list[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

void	st_print_export_list(t_symtab **export_list)
{
	size_t	i;

	i = 0;
	while (export_list[i])
	{
		if (export_list[i]->value)
			printf("declare -x %s=\"%s\"\n", export_list[i]->key, \
			export_list[i]->value);
		else
			printf("declare -x %s\n", export_list[i]->key);
		i++;
	}
}

void	st_display_export_list(t_symtab *symtab)
{
	t_symtab	**export_list;
	t_symtab	*node;
	size_t		count;
	size_t		i;

	count = symtab_count_nodes(symtab);
	export_list = malloc(sizeof(t_symtab *) * (count + 1));
	if (!export_list)
		perror("Export: ERROR TO BE HANDLED");
	i = 0;
	node = symtab;
	while (i < count)
	{
		export_list[i] = node;
		node = node->next;
		i++;
	}
	export_list[i] = NULL;
	st_sort_export_list(export_list);
	st_print_export_list(export_list);
	free(export_list);
}

int	st_is_valid_var_name(char *var_name)
{
	/* ... */
	if (var_name)	// delete
		return (1);
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
		ft_strlen(arg - len_var_name - 1));
		// if (!var_value -> malloc error?? how does it differ from no '='?)
	else
		var_value = NULL;
	return (var_value);
}

int	bi_export(char **args, t_symtab *symtab)
{
	size_t		i;
	char		*var_name;
	char		*var_value;
	t_symtab	*node;

	i = 1;
	// HOW DO I KNOW IT WORKED? EXIT CODES...
	while (args[i])
	{
		var_name = get_var_name(args[i]);
		// if (!var_name -> malloc error)
		printf("var_name is %s\n", var_name);
		if (st_is_valid_var_name(var_name) == 0)
			printf("export: \'%s\': not a valid identifier\n", var_name);
		else
		{
			var_value = get_var_value(args[i]);
			node = symtab_get_node(symtab, var_name);
			if (!node)
				symtab = symtab_add_variable(symtab, args[i]);
				// check that it really adds NULL if no '='
			else if (node && var_value)
				symtab_update_value(node, var_value);
		}
		i++;
	}
	if (!args[1])
		st_display_export_list(symtab);
	return (0);	// adjust
}
