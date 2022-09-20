#include "minishell.h"
#include "parser.h"
#include "libft.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>

int	executor(t_token_lst *input, t_symtab *symtab);
void	signal_handler(int sig);

int	is_interupt = 0;
int main(void)
{
	t_token_lst	*token_head;
	t_symtab	*symtab;
	int			exit_n;
	char		*rline;

	symtab = init_symbol_table();
	if (!symtab)
		return (0);
	token_head = NULL;
	exit_n = 0;
	rline = NULL;
	signal(SIGINT, signal_handler);
	while (1)
	{
	if (is_interupt == SIGQUIT)
		break ;
	if (is_interupt == SIGINT)
	{
		is_interupt = 0;
		rl_replace_line("", 1);
	}
	rline = readline("hello> ");
	if (*rline)
		add_history(rline);
	parser(&token_head, rline, symtab, &exit_n);
    if (token_head)
			exit_n = executor(token_head, symtab);
    free_list(&token_head);
	}
	symtab_erase_and_free(&symtab);
	clear_history();
	free(rline);
	free_list(&token_head);
	return (0);
}

void	signal_handler(int sig)
{
		is_interupt = sig;
}
