/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   var_exp_word.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmonfrin <dmonfrin@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/04 15:08:37 by dmonfrin      #+#    #+#                 */
/*   Updated: 2022/10/19 14:01:25 by dmonfrin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "parser.h"
#include "libft.h"

static char	*st_quote_join(char *new_var, char *var, int i)
{	
	char	*joined;

	joined = save_previus(new_var, var, i);
	joined = save_previus(joined, "'", 1);
	return (joined);
}

/*
    This function include each words inside dobble quotes es:
    ls     ff  = 'ls'     'ff'  
	$emptyvar="$emptyvar"
*/
char	*var_quote_incl(char *var)
{
	char	*new_var;
	int		i;

	i = 0;
	new_var = NULL;
	while (var[i])
	{
		if (var[i] != ' ')
		{
			new_var = st_quote_join(new_var, var, i);
			if (!new_var)
				return (NULL);
			var += i;
			i = 0;
			while (var[i] && var[i] != ' ')
				i++;
			new_var = st_quote_join(new_var, var, i);
			if (!new_var)
				return (NULL);
			var += i;
			i = -1;
		}
		i++;
	}
	return (save_previus(new_var, var, i));
}

char	*var_dquote_incl(char *var)
{
	char	*joined;

	joined = ft_strdup("\"");
	joined = save_previus(joined, var, ft_strlen(var));
	joined = save_previus(joined, "\"", 1);
	return (joined);
}

char	*var_exclude(char *str, char *var)
{
	char	*new_str;

	if (!*var)
	{
		free (var);
		return (str);
	}
	new_str = save_previus(str, "\"'", 2);
	new_str = ft_strjoinfree(new_str, var);
	new_str = save_previus(new_str, "'\"", 2);
	return (new_str);
}
