/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main_support.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmonfrin <dmonfrin@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/04 15:03:18 by dmonfrin      #+#    #+#                 */
/*   Updated: 2022/10/20 15:09:55 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <unistd.h>
#include "parser.h"
#include "builtin.h"
#include "libft.h"
#include "executor.h"
#include "symtab.h"

void	signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		rl_replace_line("", 0);
		ft_putstr_fd("\n", STDERR_FILENO);
		rl_on_new_line();
		rl_redisplay();
		g_signal = signum;
	}
}

/*
    this function set the value of readline and signal:
    rl_cath_signal set to 0 doesn't allow readline to catch signal;
    rl_outstream set where readline diplay the output;
    sa->sa_handler set the function where the signal is sent;
*/
void	sigred_init(struct sigaction *sa, t_token_lst **head)
{
	*head = NULL;
	rl_catch_signals = 0;
	rl_outstream = stderr;
	ft_bzero(&(sa->sa_mask), sizeof(sa->sa_mask));
	sa->sa_handler = signal_handler;
	sigaction(SIGINT, sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	sig_init_action(int *exit_n, struct sigaction *sa)
{
	g_signal = 0;
	*exit_n = 1;
	ft_bzero(&(sa->sa_mask), sizeof(sa->sa_mask));
}

void	execution(struct sigaction *sa, t_token_lst **head, t_symtab **symtab,
		int *exit_n)
{
	int	exit_n_prev;

	signal(SIGINT, SIG_IGN);
	exit_n_prev = *exit_n;
	*exit_n = executor(*head, symtab);
	if (*exit_n == BI_EXITED * 2)
		*exit_n = exit_n_prev + BI_EXITED;
	if (*exit_n == 130)
		ft_putstr_fd("\n", STDERR_FILENO);
	if (*exit_n == 131)
		ft_putstr_fd("Quit: 3\n", STDERR_FILENO);
	sigaction(SIGINT, sa, NULL);
	free_list(head);
}

void	free_all_exit(t_token_lst **head, t_symtab **symtab, char *rline, \
int *exit_n)
{
	free(rline);
	free_list(head);
	rl_clear_history();
	symtab_erase_and_free(symtab);
	if (*exit_n < BI_EXITED)
		ft_putstr_fd("exit\n", STDERR_FILENO);
	else
		*exit_n -= BI_EXITED;
}
