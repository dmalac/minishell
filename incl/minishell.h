/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmonfrin <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/16 13:47:16 by dmonfrin      #+#    #+#                 */
/*   Updated: 2022/09/16 13:47:18 by dmonfrin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "libft.h"
# include "symtab.h"
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>

enum e_token_type{
	WORD,
	GRT_TH,
	SMLR_TH,
	DGRT_TH,
	DSML_TH,
	EMPTY,
	PIPE,
};

enum e_basic_exit{
	ERROR = -1,
	SUCCESS = 1,
};

typedef struct s_token_lst{
	char				*content;
	int					token_type;
	struct s_token_lst	*next;

}	t_token_lst;

t_token_lst	**parser(t_token_lst **head, char *string, t_symtab *symtab,
				int *exit_n);

#endif
