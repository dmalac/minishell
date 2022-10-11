/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main_support.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmonfrin <dmonfrin@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/04 15:03:18 by dmonfrin      #+#    #+#                 */
/*   Updated: 2022/10/11 17:33:42 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "builtin.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include "executor.h"

void	signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		rl_replace_line("", 0);
		write(STDERR_FILENO, "\n", 1);
		rl_on_new_line();
		rl_redisplay();
		g_signal = signum;
	}
}

/* ************************************************************************** */
/*                                                                            */
/* this function set the value of readline and signal:                        */
/* rl_cath_signal set to 0 doesn't allow readline to catch signal;            */
/* rl_outstream set where readline diplay the output;                         */
/* sa->sa_handler set the function where the signal is sent;                  */
/*                                                                            */
/* ************************************************************************** */
void	sigred_init(struct sigaction *sa)
{
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
	signal(SIGINT, SIG_IGN);
	*exit_n = executor(*head, symtab);
	if (*exit_n == 130 || *exit_n == 131)
		write(STDERR_FILENO, "\n", 1);
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
		write(STDERR_FILENO, "exit\n", 5);
	else
		*exit_n -= BI_EXITED;
}
