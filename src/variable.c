/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   variable.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmonfrin <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/16 13:42:29 by dmonfrin      #+#    #+#                 */
/*   Updated: 2022/09/16 13:42:31 by dmonfrin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "error.h"
#include "parser.h"

int	ft_isvar(char c)
{
	return (ft_isalnum(c) || c == '_');
}

int	end_variable_set(char *str, int *prev)
{
	int	i;

	i = *prev + 1;
	if (str[i] == '?')
		return (i + 1);
	while (str[i] && ft_isvar(str[i]))
		i++;
	*prev = -1;
	return (i);
}

static char	*var_search(char *variable, int sz, t_symtab *symtab)
{
	int		size;
	char	*key;

	key = ft_substr(variable, 0, sz);
	if (!key)
		return (NULL);
	size = sz + 1;
	while (symtab)
	{
		if (ft_strncmp(key, symtab->key, size) == 0)
		{
			free(key);
			return (ft_strdup(symtab->value));
		}
		symtab = symtab->next;
	}
	free(key);
	return (ft_strdup(""));
}

char	*var_exp(char *var, int state, t_symtab *symtab)
{
	int		i;
	char	*content;

	content = NULL;
	i = 0;
	while (var[i + 1] && ft_isvar(var[i + 1]))
		i++;
	content = var_search(var + 1, i, symtab);
	if (!content)
		return (NULL);
	if (!*content && state == st_word)
	{
		free_set_null(content);
		return (ft_substr(var, 0, i + 1));
	}
	return (content);
}

char	is_empty_variable(char *str)
{
	return (str[0] == '$' && (ft_isalpha(str[1]) || str[1] == '_'));
}
