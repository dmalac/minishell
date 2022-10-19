/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmonfrin <dmonfrin@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/04 15:11:18 by dmonfrin      #+#    #+#                 */
/*   Updated: 2022/10/04 15:11:19 by dmonfrin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

typedef struct s_state{
	char	*buffer;
	int		prv_state;
	int		state;
	int		pos;
}	t_state;

typedef enum e_t_state_t
{
	blank,
	st_pipe,
	st_grt_th,
	st_smlr_th,
	st_s_quote,
	st_d_quote,
	st_eof,
	st_word,
	st_trimmer,
}	t_state_t;

/* parser.c */
t_token_lst	**parser(t_token_lst **head, char *string, t_symtab *symtab, \
int *exit_n);
/* parser_quote.c */
int			dub_quote(char *tokn, t_state *state, int *exit_n);
int			sin_quote(char *token, t_state *state, int *exit_n);
/* parser_word.c */
int			word(char *token, t_state *state, int *exit_n);
/* token_list_moves.c */
t_token_lst	*create_token(char *buffer, t_state_t state);
void		free_list(t_token_lst **lst);
int			ft_lsttadd_back(t_token_lst **lst, t_token_lst *new);
/* token_separation.c */
void		create_token_list(char *token, t_token_lst **head, int *exit_n);
int			type_of_state(char c);
/* token_split.c */
char		**raw_token_split(char *line, int *exit_n);
/* token_utility.c */
char		*free_set_null(char *str);
int			ft_isvar(char c);
char		ft_isvar_empty(char *str);
char		*ft_strjoinfree(char *s1, char *s2);
void		**ft_free_mem(char **str_of_str, int count);
/* var_exp_utility.c */
int			isallsp(char *var);
int			quote_end_set(char *string, int *i);
char		*save_previus(char *string_alloc, char *string, int i);
char		*set_beginning(char c);
int			var_end_set(char *str, int *prev);
/* var_exp_word.c */
char		*var_quote_incl(char *var);
char		*var_dquote_incl(char *var);
char		*var_exclude(char *str, char *var);
/* var_exp.c*/
char		*var_expantion(char *str, t_symtab *symtab);

#endif
