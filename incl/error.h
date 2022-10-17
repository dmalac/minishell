/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmonfrin <dmonfrin@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/04 15:11:41 by dmonfrin      #+#    #+#                 */
/*   Updated: 2022/10/04 15:11:43 by dmonfrin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

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

#endif
