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

# define CHILD 0
# define PARENT 1
# define BI_EXITED 10000

int		bi_cd(char *address, t_symtab *symtab);
int		bi_echo(char **args);
int		bi_env(t_symtab *symtab);
int		bi_export(char **args, t_symtab *symtab);
int		bi_exit(char **args, int who);
int		bi_pwd(void);
int		bi_unset(char **args, t_symtab **symtab);
/* builtin_utils.c */
void	builtin_error(char *bi_name, char *error_cause, char *error_msg);
int		execute_builtin(char **args, t_symtab **symtab, int who);
int		is_builtin(char *cmd);
int		bi_is_valid_var_name(char *bi_name, char *var_name);
/* builtin_export_lst.c */
int		bi_display_export_list(t_symtab *symtab);
/* builtin_export_var.c */
int		bi_get_var_name(char *arg, char **var_name);
// int		bi_get_var_val(char *arg, char **var_val);
int		bi_export_variable(char *arg, char **var_name, char **var_val, \
t_symtab *symtab);

#endif