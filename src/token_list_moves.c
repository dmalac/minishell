/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   token_list_moves.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmonfrin <dmonfrin@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/04 15:06:22 by dmonfrin      #+#    #+#                 */
/*   Updated: 2022/10/04 15:06:25 by dmonfrin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int	token_types(char *buffer, t_state_t state)
{	
	if (state == st_word)
		return (WORD);
	if (state == st_pipe)
		return (PIPE);
	if (state == st_grt_th)
	{
		if (ft_strlen(buffer) == 1)
			return (GRT_TH);
		return (DGRT_TH);
	}
	if (state == st_smlr_th)
	{
		if (ft_strlen(buffer) == 1)
			return (SMLR_TH);
		return (DSML_TH);
	}
	if (state == st_trimmer)
		return (EMPTY);
	return (-1);
}

t_token_lst	*create_token(char *buffer, t_state_t state)
{
	t_token_lst	*new_element;
	int			token_type;

	token_type = token_types(buffer, state);
	new_element = malloc(sizeof(t_token_lst));
	if (!new_element)
	{
		free(buffer);
		return (NULL);
	}
	new_element->next = NULL;
	new_element->content = buffer;
	new_element->token_type = token_type;
	return (new_element);
}

int	ft_lsttadd_back(t_token_lst **lst, t_token_lst *new)
{
	t_token_lst	*place_holder;

	if (new == NULL)
	{
		free_list(lst);
		return (0);
	}	
	if (*lst == NULL)
		*lst = new;
	else
	{
		place_holder = *lst;
		while (place_holder->next != NULL)
			place_holder = place_holder->next;
		place_holder->next = new;
	}
	return (1);
}

void	free_list(t_token_lst **lst)
{
	t_token_lst	*place_holder;
	t_token_lst	*index;

	if (lst != NULL)
	{
		if (*lst != NULL)
		{
			place_holder = *lst;
			index = *lst;
			while (index != NULL)
			{
				index = index->next;
				free(place_holder->content);
				free(place_holder);
				place_holder = index;
			}
		}
		*lst = NULL;
	}
}
