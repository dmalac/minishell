/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmonfrin <dmonfrin@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/04 15:03:56 by dmonfrin      #+#    #+#                 */
/*   Updated: 2022/10/04 15:03:58 by dmonfrin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include "main_support.h"

int	main(void)
{
	t_token_lst			*token_head;
	t_symtab			*symtab;
	int					exit_n;
	char				*rline;
	struct sigaction	sa;

	symtab = init_symbol_table();
	exit_n = 0;
	while (1)
	{
		rline = readline(var_search("PS1", 3, symtab));
		if (!rline && g_signal != SIGINT)
			break ;
		if (g_signal == SIGINT)
			sig_init_action(&exit_n, &sa);
		if (rline && *rline)
			add_history(rline);
		if (rline && *rline)
			parser(&token_head, rline, symtab, &exit_n);
		if (token_head)
			execution(&sa, &token_head, symtab, &exit_n);
		free(rline);
	}
	free_all_exit(&token_head, &symtab, rline);
	return (exit_n);
}
