#include "minishell.h"
#include "parser.h"
#include "builtin.h"
#include "libft.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>

int	executor(t_token_lst *input, t_symtab *symtab);
void	signal_handler(int signum);

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
	
	rl_catch_signals = 0;
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
	
	while (1)
	{
		rline = NULL;
		rline = readline("bash$ ");
		if (!rline && !is_interupt)
			break ;
		 if (is_interupt == SIGINT)
		 	is_interupt = 0;
		if (is_interupt == SIGQUIT)
		 	is_interupt = 0;
		if (rline && *rline)
		{	
			add_history(rline);
			parser(&token_head, rline, symtab, &exit_n);
		}
		if (token_head)
			exit_n = executor(token_head, symtab);
			
		free_list(&token_head);
		free(rline);
		
	}
	symtab_erase_and_free(&symtab);
	rl_clear_history();
	free(rline);
	free_list(&token_head);
	write(STDOUT_FILENO, "exit\n", 5);
	return (0);
}

void	signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		rl_replace_line("",0);
		write(1, "\n", 1);
		rl_on_new_line();
		rl_redisplay();
	}
	is_interupt = signum;
}
