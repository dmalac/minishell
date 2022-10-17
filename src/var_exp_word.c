/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   var_exp_word.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmonfrin <dmonfrin@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/04 15:08:37 by dmonfrin      #+#    #+#                 */
/*   Updated: 2022/10/17 11:29:23 by dmonfrin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "parser.h"
#include "libft.h"

static char	*st_quote_join(char *new_var, char *var, int i)
{
	char	*joined;

	joined = save_previus(new_var, var, i);
	if (var[i] == ' ')
		joined = save_previus(joined, "\" ", 2);
	else
		joined = save_previus(joined, "\"", 1);
	return (joined);
}

static char	*st_var_division(char *var)
{
	char	*new_var;
	int		i;

	i = 0;
	new_var = NULL;
	while (var[i])
	{
		if (var[i] == ' ')
		{
			new_var = save_previus(new_var, var, i + 1);
			if (!new_var)
				return (NULL);
			var += i + 1;
			i = -1;
			while (*var && *var == ' ')
				var++;
		}
		i++;
	}
	return (save_previus(new_var, var, i));
}

static int	st_set(int *i)
{
	int	numb;

	numb = *i + 1;
	*i = -1;
	return (numb);
}

static char	*st_var_inclusion(char *var)
{
	char	*new_var;
	int		i;

	i = 0;
	new_var = set_beginning(*var);
	if (!new_var)
		return (NULL);
	if (*var == ' ')
		var++;
	while (var[i])
	{
		if (var[i] == ' ')
		{
			new_var = st_quote_join(new_var, var, i);
			if (!new_var)
				return (NULL);
			var += st_set(&i);
			if (*var)
				new_var = save_previus(new_var, "\"", 1);
		}
		i++;
	}
	if (var[i - 1] == ' ')
		return (new_var);
	return (st_quote_join(new_var, var, i));
}

char	*var_fixing(char *var)
{
	char	*div_var;
	char	*new_var;

	if (var[0] == '$')
	{	
		new_var = ft_strjoin("\"", var);
		if (!new_var)
			return (NULL);
		new_var = save_previus(new_var, "\"", 1);
	}
	else
	{
		div_var = st_var_division(var);
		if (!div_var)
			return (NULL);
		new_var = st_var_inclusion(div_var);
		free(div_var);
	}
	return (new_var);
}
