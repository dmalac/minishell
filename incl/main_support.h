/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main_support.h                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmonfrin <dmonfrin@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/04 15:10:40 by dmonfrin      #+#    #+#                 */
/*   Updated: 2022/10/11 17:40:44 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_SUPPORT_H
# define MAIN_SUPPORT_H

# include "minishell.h"

void	signal_handler(int signum);
void	sigred_init(struct sigaction *sa, t_token_lst **head);
void	sig_init_action(int *exit_n, struct sigaction *sa);
void	execution(struct sigaction *sa, t_token_lst **head, t_symtab **symtab,
			int *exit_n);
void	free_all_exit(t_token_lst **head, t_symtab **symtab, char *rline, \
int *exit_n);

#endif
