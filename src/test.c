#include "minishell.h"
#include "parser.h"
#include "libft.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

int	executor(t_token_lst *input, t_symtab *symtab);

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

	while (1)
	{

	rline = readline("hello> ");
	if (*rline)
		add_history(rline);
	parser(&token_head, rline, symtab, &exit_n);
    if (token_head)
			exit_n = executor(token_head, symtab);
    free_list(&token_head);
	}
	symtab_erase_and_free(&symtab);
	return (0);
}
