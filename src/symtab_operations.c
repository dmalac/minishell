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

/* 
	This function finds a node in the symbol table containing the variable the 
	name of which is provided as the argument "key". It returns a char *pointer 
	to the value of that variable.
 */
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

/* 
	This function finds a node in the symbol table containing the variable the 
	name of which is provided as the argument "key". It returns a pointer to the 
	node containing that variable.
 */
t_symtab	*symtab_get_node(t_symtab *symtab, char *key)
{
	t_symtab	*node;

	node = symtab;
	while (node && ft_strncmp(key, node->key, ft_strlen(node->key) + 1) != 0)
		node = node->next;
	return (node);
}

/* 
	This value updates the value in a specific node in the symbol table.
*/
void	symtab_update_value(t_symtab *node, char *new_value)
{
	free(node->value);
	node->value = new_value;
}

/* 
	This function finds a node corresponding to the variable name passed to the 
	function as the argument "key" and changes its value.
*/
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

/* 
	This function counts the number of nodes in the symbol table linked list.
*/
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
