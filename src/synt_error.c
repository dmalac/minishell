/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   synt_red_error.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmonfrin <dmonfrin@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/04 15:09:45 by dmonfrin      #+#    #+#                 */
/*   Updated: 2022/10/04 15:09:47 by dmonfrin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include "error.h"
#include "minishell.h"

int	syntax_error(int error_type, int *exit_n)
{
	const char	error[8][10] = {"`newline'", "`>'", "`<'", "`>>'",
		"`<<'", "`|'", "`\"'", "`''" };

	ft_putstr_fd("mish: syntax error near unexpected token ", STDERR_FILENO);
	ft_putendl_fd((char *)error[error_type], STDERR_FILENO);
	*exit_n = 258;
	return (ERROR);
}

int	malloc_error(int *exit_n)
{
	perror("mish: ");
	*exit_n = errno;
	return (ERROR);
}
