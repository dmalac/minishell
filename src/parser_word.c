/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   word_parsing.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmonfrin <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/16 13:44:55 by dmonfrin      #+#    #+#                 */
/*   Updated: 2022/09/16 13:44:57 by dmonfrin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "error.h"

static char	*store_previous(char *token, char **str, int i)
{
	char	*new_str;

	if (*str == NULL)
		new_str = ft_substr(token, 0, i);
	else if (is_empty_var(*str))
	{	
		free(*str);
		new_str = ft_substr(token, 0, i);
	}
	else
		new_str = ft_strjoinfree(*str, ft_substr(token, 0, i));
	*str = new_str;
	return (*str);
}

static char	*var_extention(char *token, char **str, int i, t_symtab *symtab)
{
	char	*variable;

	variable = var_expantion(token + i, st_word, symtab);
	if (!variable)
		return (free_set_null(*str));
	if (i > 0)
	{	
		if (!store_previous(token, str, i))
			return (free_set_null(variable));
	}
	if (*str == NULL)
		return (variable);
	else if (!is_empty_var(*str) && is_empty_var(variable))
	{	
		free(variable);
		return (*str);
	}
	else if (is_empty_var(*str) && !is_empty_var(variable))
	{	
		free(*str);
		return (variable);
	}
	else
		return (ft_strjoinfree(*str, variable));
	return (*str);
}

static char	*exit_num_exp(char *token, char **str, int i, int *exit_n)
{
	char	*exit_code;

	exit_code = ft_itoa(*exit_n);
	if (!exit_code)
		return (free_set_null(*str));
	if (i > 0)
		if (!store_previous(token, str, i))
			return (free_set_null(exit_code));
	if (*str == NULL)
		return (exit_code);
	else if (is_empty_var(*str))
	{	
		free(*str);
		return (exit_code);
	}
	else
		return (ft_strjoinfree(*str, exit_code));
	return (*str);
}

/* an empty variable alone or a sequenze of just empty veriables 	*/
/* return without expantion. in other combinations it is erased		*/
static char	*extract_word(char *tokn, t_symtab *symtab, int *exit_n)
{
	int		i;
	char	*str;

	i = 0;
	str = NULL;
	while (tokn[i] && type_of_state(tokn[i]) == st_word)
	{
		if (tokn[i] == '$' && is_begin_var(tokn[i + 1]))
		{
			if (tokn[i + 1] == '?')
				str = exit_num_exp(tokn, &str, i, exit_n);
			else
				str = var_extention(tokn, &str, i, symtab);
			if (!str)
				return (NULL);
			tokn += end_var_set(tokn, &i);
		}
		i++;
	}
	if (str == NULL || (is_empty_var(str) && i > 0))
	{	
		free_set_null(str);
		return (ft_substr(tokn, 0, i));
	}
	return (ft_strjoinfree(str, ft_substr(tokn, 0, i)));
}

/* save a word token or concatenate it with another word token:	*/
/* if there is a previus word token and the new one is an just	*/
/* an empty variable it frees the new one						*/
int	word(char *token, t_state *st, t_symtab *symtab, int *exit_n)
{
	char	*ex_word;

	ex_word = extract_word(token, symtab, exit_n);
	if (!ex_word)
		return (malloc_error(exit_n));
	if (st->prv_state != st_word || st->buffer == NULL)
		st->buffer = ex_word;
	else if (is_empty_var(ex_word))
		free(ex_word);
	else
		st->buffer = ft_strjoinfree(st->buffer, ex_word);
	if (st->buffer == NULL)
		return (malloc_error(exit_n));
	st->prv_state = st->state;
	while (st->prv_state == type_of_state(token[st->pos]))
		st->pos++;
	if (is_empty_var(st->buffer))
		st->prv_state = st_trimmer;
	return (SUCCESS);
}
