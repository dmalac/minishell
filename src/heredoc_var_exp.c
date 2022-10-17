/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc_child.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/04 18:47:02 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/10/04 18:47:02 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "libft.h"
#include "symtab.h"

/* 
	The function locates the '$' character preceding a valid variable name in a 
	here_doc line and returns the length of the string up to that character
 */
static size_t	st_find_next_valid_var(char *line, size_t i)
{
	size_t	len;

	len = 0;
	while (line[i + len])
	{
		if (line[i + len] == '$' && line[i + len + 1] && \
		(line[i + len + 1] == '_' || ft_isalpha(line[i + len + 1]) == 1))
			return (len);
		len++;
	}
	return (len);
}

/* 
	The function locates the '$' character preceding a valid variable name in 
	the string received as argument and creates a substring of that string up to 
	that character, which it returns.
 */
static char	*st_get_str_up_to_var(char *line, size_t *i)
{
	char	*str;
	size_t	len;

	len = st_find_next_valid_var(line, *i);
	if (len == 0)
		str = ft_strdup("");
	else
		str = ft_substr(line, *i, len);
	if (!str)
		return (NULL);
	*i = *i + len;
	return (str);
}

/* 
	The function receives a pointer to a '$' character in a here_doc line and 
	extracts the variable name, which it returns
 */
static char	*st_get_var_name(char *line, size_t *start)
{
	char	*var_name;
	size_t	len;

	len = 0;
	while (line[len] && (ft_isalnum(line[len]) == 1 || line[len] == '-'))
		len++;
	var_name = ft_substr(line, 0, len);
	*start = *start + len + 1;
	return (var_name);
}

/* 
	This function attempts to retrieve and returns a value form the symbol table 
	corresponding to the variable name contained in a here_doc line. If no such 
	variable is in the symbol table, an empty string is returned
 */
static char	*st_get_var_value(char *line, size_t *i, t_symtab *symtab)
{
	char	*var_name;
	char	*var_val;
	char	*temp;

	var_name = st_get_var_name(line + *i + 1, i);
	if (!var_name)
		return (NULL);
	temp = symtab_get_value(symtab, var_name);
	if (!temp)
		var_val = ft_strdup("");
	else
		var_val = ft_strdup(temp);
	return (var_val);
}

/* 
	This function verifies whether the line inserted by the user in the context 
	of here_doc contains variables that need to be expanded. If so, it performs 
	such expansion and returns a new line.
 */
char	*heredoc_expand_var(char *line, t_symtab **symtab, t_heredoc **hd_list, \
t_heredoc *hd_node)
{
	size_t	i;
	char	*new_line;
	char	*temp;

	if (ex_contains_char(line, '$') == 0)
		return (line);
	i = 0;
	new_line = ft_strdup("");
	while (line[i] && new_line)
	{
		if (line[i] == '$' && line[i + 1] && (line[i + 1] == '_' || \
		ft_isalpha(line[i + 1]) == 1))
			temp = st_get_var_value(line, &i, *symtab);
		else
			temp = st_get_str_up_to_var(line, &i);
		if (!temp)
			heredoc_error_and_exit(symtab, hd_list, hd_node);
		new_line = ft_strjoin(new_line, temp);
		free (temp);
	}
	if (!new_line)
		heredoc_error_and_exit(symtab, hd_list, hd_node);
	free (line);
	line = NULL;
	return (new_line);
}
