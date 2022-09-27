/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/31 18:51:46 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/09/16 13:28:56 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include <stdlib.h>
# include "symtab.h"

int		bi_cd(char *address, t_symtab *symtab);
int		bi_echo(char **args);
int		bi_env(t_symtab *symtab);
int		bi_export(char **args, t_symtab *symtab);
int		bi_exit(char **args);
int		bi_pwd(void);
int		bi_unset(char **args, t_symtab *symtab);
/* builtin_utils.c */
int		execute_builtin(char **args, t_symtab *symtab);
int		is_builtin(char *cmd);
/* builtin_export_lst.c */
int		display_export_list(t_symtab *symtab);
/* builtin_export_var.c */
int		is_valid_var_name(char *var_name);
char	*get_var_name(char *arg);
char	*get_var_value(char *arg);

#endif