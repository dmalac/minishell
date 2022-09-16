/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmonfrin <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/16 13:43:46 by dmonfrin      #+#    #+#                 */
/*   Updated: 2022/09/16 13:43:47 by dmonfrin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H
# include "libft.h" 
# include "minishell.h"

enum e_error{
	err_new_line,
	err_s_grt,
	err_s_smlr,
	err_d_grt,
	err_d_smlr,
	err_pip,
	err_noclose_d,
	err_noclose_s,
	syntact_error,
};

int			malloc_error(int *exit_n);
int			syntax_error(int error_type, int *exit_n);
int			redirect_error(char *content, int *exit_n);

#endif
