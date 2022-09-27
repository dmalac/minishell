/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   symtab_operations.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/26 17:56:05 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/09/26 17:56:05 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include "symtab.h"
#include "libft.h"

/* finds and returns a pointer to the value belonging to a particular 
	variable name (parameter "key") */
char	*symtab_get_value(t_symtab *symtab, char *key)
{
	t_symtab	*node;

	node = symtab;
	while (node && ft_strncmp(key, node->key, ft_strlen(node->key) + 1) != 0)
		node = node->next;
	if (node)
		return (node->value);
	else
		return (NULL);
}

/* finds and returns a pointer to the node containing a particular 
	variable name (parameter "key") */
t_symtab	*symtab_get_node(t_symtab *symtab, char *key)
{
	t_symtab	*node;

	node = symtab;
	while (node && ft_strncmp(key, node->key, ft_strlen(node->key) + 1) != 0)
		node = node->next;
	return (node);
}

/* updates the value in a specific node */
void	symtab_update_value(t_symtab *node, char *new_value)
{
	free(node->value);
	node->value = new_value;
}

/* changes the value in a node corresponding to the variable name passed as
	parameter "key" */
void	symtab_change_value(t_symtab *symtab, char *key, char *new_value)
{
	t_symtab	*node;

	node = symtab;
	while (node && ft_strncmp(node->key, key, ft_strlen(node->key) + 1) != 0)
		node = node->next;
	if (node)
	{
		free(node->value);
		node->value = new_value;
	}
}

/* counts the number of nodes in the linked list */
size_t	symtab_count_nodes(t_symtab *node)
{
	size_t	count;

	count = 0;
	while (node)
	{
		count++;
		node = node->next;
	}
	return (count);
}
