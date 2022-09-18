/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmonfrin <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/16 13:42:13 by dmonfrin      #+#    #+#                 */
/*   Updated: 2022/09/16 13:42:15 by dmonfrin      ########   odam.nl         */
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

enum e_state
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
};

char		**raw_token_split(char *line, int *exit_n);
void		create_token_list(char *token,
				t_token_lst **head, t_symtab *symtab, int *exit_n);
t_token_lst	*create_token(char *buffer, int state);
void		free_list(t_token_lst **lst);
int			ft_lsttadd_back(t_token_lst **lst, t_token_lst *new);
char		**raw_token_split(char *line, int *exit_n);
char		*ft_strjoinfree(char *s1, char *s2);
int			type_of_state(char c);
int			word(char *token, t_state *state, t_symtab *symtab, int *exit_n);
int			dub_quote(char *tokn, t_state *state, t_symtab *symtab,
				int *exit_n);
int			sin_quote(char *token, t_state *state, int *exit_n);
int			ft_str_strlen(char **tokens);
int			end_var_set(char *str, int *prev);
char		*var_expantion(char *var, int state, t_symtab *symtab);
int			ft_isvar(char c);
void		double_free(char *str, t_token_lst **head);
char		is_empty_var(char *str);
int			is_begin_var(char c);
char		*free_set_null(char *str);

void		**ft_free_mem(char **str_of_str, int count);

#endif
