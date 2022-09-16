#include "minishell.h"
#include "parser.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>


int main()
{
	t_token_lst *token_head;
	t_symtab symtab;
	int exit_n;
	token_head = NULL;
	exit_n = 0;
	symtab.key = ft_strdup("hello");
	symtab.value = ft_strdup("ppp");
	symtab.next = NULL;
	if(!symtab.key || !symtab.value)
		return 0;
	char *rline = NULL;

	while (1)
	{

	rline = readline("hello");
	if (*rline)
		add_history(rline);
	parser(&token_head, rline, &symtab, &exit_n);
    
    free_list(&token_head);
	}
	free(symtab.key);
	free(symtab.value);
    return (0);}