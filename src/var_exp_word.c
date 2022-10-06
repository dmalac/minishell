/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   var_exp_word.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmonfrin <dmonfrin@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/04 15:08:37 by dmonfrin      #+#    #+#                 */
/*   Updated: 2022/10/06 16:42:59 by dmonfrin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static char	*quote_join(char *new_var, char *var, int i)
{
	char	*joined;

	joined = save_previus(new_var, var, i);
	if (var[i] == ' ')
		joined = save_previus(joined, "\" ", 2);
	else
		joined = save_previus(joined, "\"", 1);
	return (joined);
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

static char	*var_division(char *var)
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

static char	*var_inclusion(char *var)
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
			new_var = quote_join(new_var, var, i);
			if (!new_var)
				return (NULL);
			var += i + 1;
			if (*var)
				new_var = save_previus(new_var, "\"", 1);
			i = -1;
		}
		i++;
	}
	new_var = quote_join(new_var, var, i);
	return (new_var);
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
		div_var = var_division(var);
		if (!div_var)
			return (NULL);
		new_var = var_inclusion(div_var);
		free(div_var);
	}
	return (new_var);
}
