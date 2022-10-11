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
#include <signal.h>
#include "main_support.h"
#include "error.h"
#include "builtin.h"

int	g_signal = 0;

static char	*user_input(t_symtab *symtab)
{
	char	*shell_name;
	char	*user_input;

	shell_name = symtab_get_value(symtab, "PS1");
	user_input = readline(shell_name);
	return (user_input);
}

int	main(void)
{
	t_token_lst			*token_head;
	t_symtab			*symtab;
	int					exit_n;
	char				*line;
	struct sigaction	sa;

	symtab = init_symbol_table();
	exit_n = 0;
	sigred_init(&sa);
	while (exit_n < BI_EXITED)
	{
		line = user_input(symtab);
		if (g_signal == SIGINT)
			sig_init_action(&exit_n, &sa);
		if (!line && !g_signal)
			break ;
		if (line && *line)
			parser(&token_head, line, symtab, &exit_n);
		if (token_head)
			execution(&sa, &token_head, &symtab, &exit_n);
		if (exit_n < BI_EXITED)
			free(line);
	}
	free_all_exit(&token_head, &symtab, line, &exit_n);
	return (exit_n);
}
