/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   quote_parsing.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmonfrin <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/16 13:41:47 by dmonfrin      #+#    #+#                 */
/*   Updated: 2022/09/16 13:41:49 by dmonfrin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "error.h"

static int	set_trimmer(char *token, char *d_word, t_state *st, int *exit_n)
{
	if (st->prv_state != st_trimmer)
	{
		free(d_word);
		while (token[st->pos] && token[st->pos] != '"')
		st->pos++;
		if (!token[st->pos])
			return (syntax_error(err_noclose_d, exit_n));
	}
	else
	{
		st->buffer = ft_strjoinfree(st->buffer, d_word);
		if (st->buffer == NULL)
			return (malloc_error(exit_n));
		while (token[st->pos] && token[st->pos] != '"')
		st->pos++;
		if (!token[st->pos])
			return (syntax_error(err_noclose_d, exit_n));
		st->prv_state = st_trimmer;
	}
	st->pos += 2;
	return (SUCCESS);
}

static char	*exit_n_proc(char *token, char **str, int i, int *exit_n)
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

static char	*extract_d_quote(char *tokn, int *exit_n)
{
	int		i;
	char	*str;

	i = 0;
	str = NULL;
	while (tokn[i] && tokn[i] != '"' )
	{
		if (tokn[i] == '$' && tokn[i + 1] == '?')
		{
			str = exit_n_proc(tokn, &str, i, exit_n);
			if (!str)
				return (NULL);
			tokn += 2;
			i = -1;
		}
		i++;
	}
	if (str == NULL)
		return (ft_substr(tokn, 0, i));
	else
		return (ft_strjoinfree(str, ft_substr(tokn, 0, i)));
}

/* if the previus token is word it conacatenate it with the content		*/
/* of the quotes, if the previus token is an empty variable, it ereses	*/
/* it and save just the content of the quotes, if the quotes are not	*/
/* close it trow a sitax error											*/
int	sin_quote(char *token, t_state *st, int *exit_n)
{
	char	*str;

	if (!*token)
		return (syntax_error(err_noclose_s, exit_n));
	while (token[st->pos] != '\'' && token[st->pos])
		st->pos++;
	if (!token[st->pos])
		return (syntax_error(err_noclose_s, exit_n));
	str = ft_substr(token, 0, st->pos);
	if (str == NULL)
		return (malloc_error(exit_n));
	if (st->prv_state == st_trimmer || !(st->buffer))
		free_set_null(st->buffer);
	if (st->prv_state != st_word || !(st->buffer))
		st->buffer = str;
	else
		st->buffer = ft_strjoinfree(st->buffer, str);
	if (st->buffer == NULL)
		return (malloc_error(exit_n));
	st->pos += 2;
	st->prv_state = st_word;
	return (SUCCESS);
}

/* if the previus token is word it conacatenate it with the content		*/
/* of the quotes, if the previus token is an empty variable, it ereses	*/
/* it and save just the content of the quotes, if the quotes are not	*/
/* close it trow a sitax error											*/
int	dub_quote(char *token, t_state *st, int *exit_n)
{
	char	*d_word;

	if (!*token)
		return (syntax_error(err_noclose_d, exit_n));
	d_word = extract_d_quote(token, exit_n);
	if (!d_word)
		return (malloc_error(exit_n));
	if (ft_isvar_empty(d_word))
		return (set_trimmer(token, d_word, st, exit_n));
	if (st->prv_state == st_trimmer || !(st->buffer))
		free_set_null(st->buffer);
	if (st->prv_state != st_word || !(st->buffer))
		st->buffer = d_word;
	else
		st->buffer = ft_strjoinfree(st->buffer, d_word);
	if (st->buffer == NULL)
		return (malloc_error(exit_n));
	while (token[st->pos] && token[st->pos] != '"')
		st->pos++;
	if (!token[st->pos])
		return (syntax_error(err_noclose_d, exit_n));
	st->prv_state = st_word;
	st->pos += 2;
	return (SUCCESS);
}
