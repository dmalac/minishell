/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_echo.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/14 18:12:08 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/09/20 17:46:37 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "builtin.h"
#include "libft.h"

static int	st_skip_n_option(char **args, size_t *i)
{
	int		nl;
	size_t	j;

	nl = 1;
	while (args[*i])
	{
		j = 0;
		if (args[*i][j] != '-')
			return (nl);
		j++;
		while (args[*i][j] == 'n')
			j++;
		if (args[*i][j])
			return (nl);
		nl = 0;
		*i += 1;
	}
	return (nl);
}

int	bi_echo(char **args)
{
	int		result;
	int		nl;
	size_t	i;

	// segfaults with NULL as input but that should never be input - TBD
	i = 1;
	nl = st_skip_n_option(args, &i);
	while (args[i])
	{
		result = printf("%s", args[i++]); // should this be ft_putstr instead?
		if (result < 0)
			return (1);
		if (args[i])	// watch out for "" (should not be NULL)
			printf(" ");
	}
	if (nl == 1)
		printf("\n");
	return (0);
}
