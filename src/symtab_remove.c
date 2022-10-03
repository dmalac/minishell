/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   symtab_remove.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/26 17:57:34 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/09/26 17:57:34 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include "symtab.h"
#include "libft.h"

/* frees allocated memory in a node that was removed from the list */
static void	st_symtab_erase_and_free_node(t_symtab **node)
{
	free((*node)->key);
	(*node)->key = NULL;
	free((*node)->value);
	(*node)->value = NULL;
	free(*node);
	(*node) = NULL;
}

void	symtab_erase_and_free(t_symtab **top)
{
	t_symtab	*node;
	t_symtab	*to_free;

	node = *top;
	while (node)
	{
		to_free = node;
		node = node->next;
		st_symtab_erase_and_free_node(&to_free);
	}
}

/* removes a node from the list */
t_symtab	*symtab_remove_node(t_symtab **top, char *key)
{
	t_symtab	*prev;
	t_symtab	*tmp;

	if (!(*top))
		return (*top);
	prev = NULL;
	tmp = *top;
	while (tmp && ft_strncmp(key, tmp->key, ft_strlen(key) + 1) != 0)
	{
		prev = tmp;
		tmp = tmp->next;
	}
	if (!tmp)
		return (*top);
	if (!prev)
		*top = (*top)->next;
	else
		prev->next = tmp->next;
	st_symtab_erase_and_free_node(&tmp);
	return (*top);
}

/* TO BE DELETED */
/* prints the whole symbol table in the format <KEY>=<VALUE> */
void	symtab_print(t_symtab *top)
{
	printf("*** SYMBOL TABLE ***\n\n");
	
	while (top)
	{
		printf("\t%s=%s\n", top->key, top->value);
		top = top->next;
	}
	
	printf("\n*** END OF SYMBOL TABLE ***\n\n");
}
