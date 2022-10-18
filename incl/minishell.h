/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmonfrin <dmonfrin@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/04 15:10:57 by dmonfrin      #+#    #+#                 */
/*   Updated: 2022/10/04 15:10:59 by dmonfrin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

extern int	g_signal;

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

typedef struct s_symtab
{
	char			*key;
	char			*value;
	struct s_symtab	*next;
}	t_symtab;

#endif
