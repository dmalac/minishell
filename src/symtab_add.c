/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   symtab_add.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/26 17:54:55 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/09/26 17:54:55 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "symtab.h"
#include "libft.h"

/* 
	This function creates and returns a new node in the symbol table with the 
	content extracted from the string passed as argument.
 */
t_symtab	*symtab_new(char *str)
{
	size_t		i;
	t_symtab	*new;

	new = malloc(sizeof(t_symtab));
	if (!str || !new)
		return (ft_putendl_fd(strerror(errno), STDERR_FILENO), NULL);
	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	new->key = ft_substr(str, 0, i);
	if (str[i] != '=')
		new->value = NULL;
	else
		new->value = ft_substr(str, i + 1, ft_strlen(str) - i - 1);
	new->next = NULL;
	return (new);
}

/* 
	The function adds a new node to the back of the symbol table linked list.
 */
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

/* 
	This function creates a new node in the symbol table with the content 
	extracted from the string passed as argument and adds the new node to the 
	back of the symbol table linked list. It returns a pointer to the newly 
	added node, or NULL if the addition was unsuccessful.
 */
t_symtab	*symtab_add_node(t_symtab **top, char *str)
{
	t_symtab	*new;

	new = symtab_new(str);
	if (new)
		symtab_add_back(top, new);
	return (new);
}

/* 
	The function adds a new variable to the symbol table with the variable name 
	and value provided as arguments. It returns 0 upon success or 1 if an error 
	occurred.
 */
int	symtab_add_var(t_symtab *symtab, char *var_name, char *var_val)
{
	t_symtab	*new;

	new = malloc(sizeof(t_symtab));
	if (!new)
		return (ft_putendl_fd(strerror(errno), STDERR_FILENO), EXIT_FAILURE);
	new->key = var_name;
	new->value = var_val;
	new->next = NULL;
	symtab_add_back(&symtab, new);
	return (EXIT_SUCCESS);
}
