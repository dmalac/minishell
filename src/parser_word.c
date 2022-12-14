/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser_word.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmonfrin <dmonfrin@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/04 15:05:27 by dmonfrin      #+#    #+#                 */
/*   Updated: 2022/10/04 15:05:31 by dmonfrin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "parser.h"
#include "libft.h"

static char	*st_exit_num_exp(char *token, char **str, int i, int *exit_n)
{
	char	*exit_code;
	char	*new_str;

	new_str = NULL;
	exit_code = ft_itoa(*exit_n);
	if (!exit_code)
		return (free_set_null(*str));
	if (i > 0)
	{
		if (*str != NULL)
			new_str = ft_strjoinfree(*str, ft_substr(token, 0, i));
		else
			new_str = ft_substr(token, 0, i);
		if (!new_str)
			return (free_set_null(exit_code));
	}
	if (new_str == NULL)
		return (exit_code);
	else
		return (ft_strjoinfree(new_str, exit_code));
	return (new_str);
}

static char	*st_extract_word(char *tokn, int *exit_n)
{
	int		i;
	char	*str;

	i = 0;
	str = NULL;
	while (tokn[i] && type_of_state(tokn[i]) == st_word)
	{
		if (tokn[i] == '$' && tokn[i + 1] == '?')
		{
			str = st_exit_num_exp(tokn, &str, i, exit_n);
			if (!str)
				return (NULL);
			tokn += i + 2;
			i = -1;
		}
		i++;
	}
	if (str == NULL)
		return (ft_substr(tokn, 0, i));
	return (ft_strjoinfree(str, ft_substr(tokn, 0, i)));
}

int	word(char *token, t_state *st, int *exit_n)
{
	char	*ex_word;

	ex_word = st_extract_word(token, exit_n);
	if (!ex_word)
		return (malloc_error(exit_n));
	if (st->prv_state != st_word || st->buffer == NULL)
	{	
		free_set_null(st->buffer);
		st->buffer = ex_word;
	}
	else
		st->buffer = ft_strjoinfree(st->buffer, ex_word);
	if (st->buffer == NULL)
		return (malloc_error(exit_n));
	st->prv_state = st->state;
	while (st->prv_state == type_of_state(token[st->pos]))
		st->pos++;
	return (SUCCESS);
}
