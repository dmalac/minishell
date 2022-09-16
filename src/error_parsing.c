/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error_parsing.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmonfrin <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/16 13:44:14 by dmonfrin      #+#    #+#                 */
/*   Updated: 2022/09/16 13:44:16 by dmonfrin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "minishell.h"
#include <errno.h>

int	syntax_error(int error_type, int *exit_n)
{
	const char	error[8][10] = {"`newline'", "`>'", "`<'", "`>>'",
		"`<<'", "`|'", "`\"'", "`''" };

	ft_putstr_fd("syntax error near unexpected token ", STDERR_FILENO);
	ft_putendl_fd((char *)error[error_type], STDERR_FILENO);
	*exit_n = 2;
	return (ERROR);
}

int	redirect_error(char *content, int *exit_n)
{
	ft_putstr_fd(content, STDERR_FILENO);
	ft_putendl_fd (": ambiguous redirect", STDERR_FILENO);
	*exit_n = 1;
	return (ERROR);
}

int	malloc_error(int *exit_n)
{
	perror(NULL);
	*exit_n = errno;
	return (ERROR);
}
