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

/* builtins */
int		bi_cd(char **args, t_symtab *symtab);
int		bi_echo(char **args);
int		bi_env(t_symtab *symtab);
int		bi_export(char **args, t_symtab *symtab);
int		bi_exit();
int		bi_pwd(void);
// int		bi_pwd(t_symtab *symtab);
int		bi_unset(char **args, t_symtab *symtab);
/* builtin_utils.c */
int		execute_builtin(char **args, t_symtab *symtab);
int		is_builtin(char *cmd);

#endif