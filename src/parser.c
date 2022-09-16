/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmonfrin <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/16 13:41:30 by dmonfrin      #+#    #+#                 */
/*   Updated: 2022/09/16 13:41:34 by dmonfrin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "error.h"

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
		if (elem->next->token_type > 0 && elem->next->token_type != EMPTY)
			return (syntax_error(elem->next->token_type, exit_n));
		if (elem->next->token_type == EMPTY)
			return (redirect_error(elem->next->content, exit_n));
	}
	if (elem->token_type == PIPE)
		if (elem->next->token_type == PIPE)
			return (syntax_error(err_pip, exit_n));
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

static void	unfold_token_list(char **tokens, t_token_lst **head,
	t_symtab *symtab, int *exit_n)
{
	int	i;

	i = 0;
	while (tokens[i])
	{
		create_token_list(tokens[i], head, symtab, exit_n);
		if (!*head)
			return ;
		i++;
	}
}

t_token_lst	**parser(t_token_lst **head, char *string,
		t_symtab *symtab, int *exit_n)
{
	char	**tokens;

	*head = NULL;
	if (string == NULL)
		return (head);
	tokens = raw_token_split(string, exit_n);
	if (!tokens || !*tokens)
		return (head);
	unfold_token_list(tokens, head, symtab, exit_n);
	ft_free_mem(tokens, ft_str_strlen(tokens));
	if (!*head)
		return (head);
	error_check(head, exit_n);
	return (head);
}
