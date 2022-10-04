/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   token_separation.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmonfrin <dmonfrin@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/04 15:06:46 by dmonfrin      #+#    #+#                 */
/*   Updated: 2022/10/04 15:06:49 by dmonfrin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "error.h"

int	type_of_state(char c)
{
	int			i;
	const char	state[] = {' ', '|', '>', '<', '\'', '"', '\0'};

	i = 0;
	while (i < 7)
	{
		if (c == state[i])
			return (i);
		i++;
	}
	return (st_word);
}

static int	matching(int prev_var, int var)
{
	if (prev_var == var || prev_var == blank)
		return (SUCCESS);
	if ((prev_var == st_word || prev_var == st_trimmer) && (var == st_word
			|| var == st_s_quote || var == st_d_quote))
		return (SUCCESS);
	return (0);
}

/* key_word includes > < |									*/
static int	key_word(char *token, t_state *st, int *exit_n)
{
	if (st->prv_state != st->state || !(st->buffer))
		st->buffer = ft_substr(token, 0, 1);
	else
		st->buffer = ft_strjoinfree(st->buffer,
				ft_substr(token, 0, 1));
	if (st->buffer == NULL)
		return (malloc_error(exit_n));
	st->prv_state = st->state;
	st->pos++;
	return (SUCCESS);
}

static int	processing(char *token, t_state *state, int *exit_n)
{
	if (state->state == st_s_quote)
		return (sin_quote(token + 1, state, exit_n));
	if (state->state == st_d_quote)
		return (dub_quote(token + 1, state, exit_n));
	if (state->state == st_word)
		return (word(token, state, exit_n));
	return (key_word(token, state, exit_n));
}

void	create_token_list(char *token, t_token_lst **head, int *exit_n)
{
	t_state			st;

	ft_bzero(&st, sizeof(t_state));
	while (token[st.pos])
	{
		st.state = type_of_state(token[st.pos]);
		if (!matching(st.prv_state, st.state))
		{	
			if (!ft_lsttadd_back(head, create_token(st.buffer, st.prv_state)))
				return ;
			st.buffer = NULL;
		}
		if (processing(token + st.pos, &st, exit_n) == ERROR)
		{
			free(st.buffer);
			free_list(head);
			return ;
		}
		token += st.pos;
		st.pos = 0;
	}
	if (st.buffer)
		ft_lsttadd_back(head, create_token(st.buffer, st.prv_state));
}
