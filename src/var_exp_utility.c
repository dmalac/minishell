/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   var_exp_utility.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmonfrin <dmonfrin@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/04 15:08:15 by dmonfrin      #+#    #+#                 */
/*   Updated: 2022/10/17 11:23:26 by dmonfrin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "libft.h"

int	var_end_set(char *str, int *prev)
{
	int	i;

	i = *prev + 1;
	while (str[i] && ft_isvar(str[i]))
		i++;
	*prev = -1;
	return (i);
}

char	*save_previus(char *string_alloc, char *string, int i)
{
	char	*joined;

	if (i == 0)
	{
		if (!string_alloc)
			return (ft_strdup(""));
		else
			return (string_alloc);
	}
	if (!string_alloc)
		return (ft_substr(string, 0, i));
	joined = ft_strjoinfree(string_alloc, ft_substr(string, 0, i));
	return (joined);
}

int	quote_end_set(char *string, int *i)
{
	char	c;
	int		count;

	count = *i + 1;
	c = string[*i];
	while (string[count] && string[count] != c)
		count++;
	if (string[count])
		count++;
	*i = -1;
	return (count);
}

char	*set_beginning(char c)
{
	if (c == ' ')
		return (ft_strdup(" \""));
	return (ft_strdup("\""));
}

int	isallsp(char *var)
{
	while (*var)
	{
		if (*var != ' ')
			return (FALSE);
		var++;
	}
	return (TRUE);
}
