/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   symtab.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/05 17:29:36 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/09/16 15:46:35 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYMTAB_H
# define SYMTAB_H

# include <stdlib.h>

typedef struct s_symtab
{
	char			*key;
	char			*value;
	struct s_symtab	*next;
}				t_symtab;

/* init.c */
t_symtab	*init_symbol_table(void);
/* symtab_add.c */
t_symtab	*symtab_new(char *str);
void		symtab_add_back(t_symtab **top, t_symtab *new);
int			symtab_add_var(t_symtab *symtab, char *var_name, char *var_val);
/* symtab_remove.c */
t_symtab	*symtab_remove_node(t_symtab **top, char *key);
void		symtab_erase_and_free(t_symtab **top);
void	symtab_print(t_symtab *top);	// to be deleted
/* symtab_operations.c */
char		*symtab_get_value(t_symtab *symtab, char *key);
t_symtab	*symtab_get_node(t_symtab *symtab, char *key);
void		symtab_change_value(t_symtab *symtab, char *key, char *new_value);
size_t		symtab_count_nodes(t_symtab *node);
void		symtab_update_value(t_symtab *node, char *new_value);

#endif