/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.h                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/07 11:54:51 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/09/12 10:46:53 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H

enum e_token_type
{
	WORD,
	GRT_TH,
	SMLR_TH,
	DGRT_TH,
	DSMLR_TH,
	EMPTY,
	PIPE,
};

typedef struct s_token_lst
{
	char				*content;
	int					token_type;
	struct s_token_lst	*next;
}				t_token_lst;


/* to be deleted */
void	print_input(t_token_lst *input);

#endif