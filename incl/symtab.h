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
# include "minishell.h"

/* init.c */
t_symtab	*init_symbol_table(void);
/* symtab_add.c */
t_symtab	*symtab_new(char *str);
void		symtab_add_back(t_symtab **top, t_symtab *new);
t_symtab	*symtab_add_node(t_symtab **top, char *str);
int			symtab_add_var(t_symtab *symtab, char *var_name, char *var_val);
/* symtab_remove.c */
t_symtab	*symtab_remove_node(t_symtab **top, char *key);
void		symtab_erase_and_free(t_symtab **top);
/* symtab_operations.c */
char		*symtab_get_value(t_symtab *symtab, char *key);
t_symtab	*symtab_get_node(t_symtab *symtab, char *key);
size_t		symtab_count_nodes(t_symtab *node);
void		symtab_update_value(t_symtab *node, char *new_value);

#endif