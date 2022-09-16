/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   symtab.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/06 11:01:37 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/09/16 15:26:49 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include "symtab.h"
#include "libft.h"

/* creates a new node */
t_symtab	*symtab_new(char *str)
{
	size_t		i;
	t_symtab	*new;
	size_t		len;

	new = malloc(sizeof(t_symtab));
	if (!str || !new)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	len = ft_strlen(str);
	new->key = ft_substr(str, 0, i);
	if (len == i + 1)
		new->value = NULL;
	else
		new->value = ft_substr(str, i + 1, len - i - 1);
	new->next = NULL;
	return (new);
}

/* adds node to the back of the list */
void	symtab_add_back(t_symtab **top, t_symtab *new)
{
	t_symtab	*node;

	if (*top == NULL)
		*top = new;
	else
	{
		node = *top;
		while (node->next)
			node = node->next;
		node->next = new;
	}
}

/* frees allocated memory in a node that was removed from the list */
static void	st_symtab_erase_and_free_node(t_symtab **node)
{
	free((*node)->key);
	free((*node)->value);
	free(*node);
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
		// free(to_free->key);
		// free(to_free->value);
		// free(to_free);
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

/* finds and returns a pointer to the value belonging to a particular 
	variable name (parameter "key") */
char	*symtab_get_value(t_symtab *symtab, char *key)
{
	t_symtab	*node;

	//what if symtab NULL?
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

	//what if symtab NULL?
	node = symtab;
	while (node && ft_strncmp(key, node->key, ft_strlen(node->key) + 1) != 0)
		node = node->next;
	if (node)
		return (node);
	else
		return (NULL);
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

	// what if symtab NULL?
	node = symtab;
	while (node && ft_strncmp(node->key, key, ft_strlen(node->key) + 1) != 0)
		node = node->next;
	if (node)
	{
		free(node->value);
		node->value = new_value;
	}
}

/* NOT SURE IT WORKS WITH PASSING ONLY *SYMTAB INSTEAD OF **SYMTAB */
t_symtab	*symtab_add_variable(t_symtab *symtab, char *str)
{
	t_symtab	*new;
	
	new = symtab_new(str);
	if (!new)
		// malloc error
		return (NULL);
	symtab_add_back(&symtab, new);
	return (symtab);
}

/* swaps values of two variables (useful for builtin function cd) */
void	symtab_swap_value(t_symtab *node1, t_symtab *node2)
{
	char	*tmp;

	tmp = node1->value;
	node1->value = node2->value;
	node2->value = tmp;
}

/* updates the PWD and OLDPWD variables */
void	symtab_update_pwd(t_symtab *oldpwd, t_symtab *pwd)
{
	char	*new_address;

	new_address = getcwd(NULL, 1);
	free(oldpwd->value);
	oldpwd->value = pwd->value;
	pwd->value = new_address;
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
