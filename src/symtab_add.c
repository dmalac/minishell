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

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
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

int	symtab_add_variable(t_symtab *symtab, char *str)
{
	t_symtab	*new;

	new = symtab_new(str);
	if (!new)
	{
		ft_putendl_fd(strerror(errno), 2);
		return (1);
	}
	symtab_add_back(&symtab, new);
	return (0);
}
