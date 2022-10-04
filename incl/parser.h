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

char		**raw_token_split(char *line, int *exit_n);
void		create_token_list(char *token, t_token_lst **head, int *exit_n);
t_token_lst	*create_token(char *buffer, t_state_t state);
void		free_list(t_token_lst **lst);
int			ft_lsttadd_back(t_token_lst **lst, t_token_lst *new);
char		**raw_token_split(char *line, int *exit_n);
char		*ft_strjoinfree(char *s1, char *s2);
int			type_of_state(char c);
int			word(char *token, t_state *state, int *exit_n);
int			dub_quote(char *tokn, t_state *state, int *exit_n);
int			sin_quote(char *token, t_state *state, int *exit_n);
int			ft_str_strlen(char **tokens);
int			ft_isvar(char c);
void		double_free(char *str, t_token_lst **head);
char		*free_set_null(char *str);
int			ft_ispcnt(char c);
void		**ft_free_mem(char **str_of_str, int count);
int			quote_end_set(char *string, int *i);
char		*save_previus(char *string_alloc, char *string, int i);
int			var_end_set(char *str, int *prev);
char		*var_fixing(char *var);
int			isallsp(char *var);
char		ft_isvar_empty(char *str);
char		*var_expantion(char *str, t_symtab *symtab);
char		*set_beginning(char c);

#endif
