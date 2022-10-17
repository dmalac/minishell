/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmonfrin <dmonfrin@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/04 15:05:55 by dmonfrin      #+#    #+#                 */
/*   Updated: 2022/10/04 15:05:57 by dmonfrin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "error.h"
#include <readline/history.h>

/* ************************************************************************** */
/*                                                                            */
/* This function is responsible to check the right token combinations,        */
/* if some combinations are wrong it trow the right error and returns -1      */
/* wrong combination:                                                         */
/* redirect or pipe at the end of the list;                                   */
/* different redirect type next to each other;                                */
/* more than 1 pipe or more than 2 redirect next to each other;               */
/* pipe and redirect next to each other;                                      */
/*                                                                            */
/* ************************************************************************** */

static int	right_value_err(t_token_lst *elem, int *exit_n)
{
	if (elem->token_type == PIPE && ft_strlen(elem->content) > 1)
		return (syntax_error(err_pip, exit_n));
	if (elem->token_type == DGRT_TH && ft_strlen(elem->content) > 3)
		return (syntax_error(err_d_grt, exit_n));
	if (elem->token_type == DGRT_TH && ft_strlen(elem->content) > 2)
		return (syntax_error(err_s_grt, exit_n));
	if (elem->token_type == DSML_TH && ft_strlen(elem->content) > 3)
		return (syntax_error(err_d_smlr, exit_n));
	if (elem->token_type == DSML_TH && ft_strlen(elem->content) > 2)
		return (syntax_error(err_s_smlr, exit_n));
	if (elem->token_type > 0 && elem->token_type != EMPTY)
	{
		if (elem->next == NULL)
			return (syntax_error(err_new_line, exit_n));
		if (elem->next->token_type == PIPE)
			return (syntax_error(err_pip, exit_n));
		if (elem->next->token_type > 0 && elem->next->token_type != EMPTY
			&& elem->token_type != PIPE)
			return (syntax_error(elem->next->token_type, exit_n));
	}	
	return (SUCCESS);
}

static void	error_check(t_token_lst **head, int *exit_n)
{
	t_token_lst	*p_h;

	if (!head || !*head)
		return ;
	p_h = *head;
	while (p_h)
	{	
		if (right_value_err(p_h, exit_n) == ERROR)
		{
			free_list(head);
			return ;
		}
		p_h = p_h -> next;
	}
}

static void	unfold_token_list(char **tokens, t_token_lst **head, int *exit_n)
{
	int	i;

	i = 0;
	while (tokens[i])
	{
		create_token_list(tokens[i], head, exit_n);
		i++;
	}
}

/* ************************************************************************** */
/*                                                                            */
/* This function is responsible to start:                                     */
/* the variable expansion;                                                    */
/* the raw token separation;                                                  */
/* the tokenization;                                                          */
/* to check syntax error and gives a working list of token or an empty (NULL) */
/* list in case of error;                                                     */
/*                                                                            */
/* ************************************************************************** */

t_token_lst	**parser(t_token_lst **head, char *raw_string,
		t_symtab *symtab, int *exit_n)
{
	char	**tokens;
	char	*var_string;

	*head = NULL;
	var_string = NULL;
	add_history(raw_string);
	if (raw_string == NULL)
		return (head);
	var_string = var_expantion(raw_string, symtab);
	if (!var_string)
	{
		malloc_error(exit_n);
		return (head);
	}
	tokens = raw_token_split(var_string, exit_n);
	free_set_null(var_string);
	if (!tokens || !*tokens)
		return (head);
	unfold_token_list(tokens, head, exit_n);
	ft_free_mem(tokens, ft_str_strlen(tokens));
	if (!*head)
		return (head);
	error_check(head, exit_n);
	return (head);
}
