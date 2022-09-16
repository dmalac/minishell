/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   raw_token_ext.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmonfrin <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/16 13:41:08 by dmonfrin      #+#    #+#                 */
/*   Updated: 2022/09/16 13:41:16 by dmonfrin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "error.h"

static int	ft_ispcnt(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

static int	len_token(char *line, int i)
{
	char	c;

	while (line[i])
	{
		if (line[i] == '"' || line[i] == '\'')
		{	
			c = line[i++];
			while (line[i] != c && line[i])
				i++;
		}
		if (ft_ispcnt(line[i]) || !line[i])
			break ;
		i++;
	}
	return (i);
}

static void	create_arr_arr(char *line, char **tokens, int count)
{
	int	start;
	int	end;
	int	i;

	i = 0;
	start = 0;
	end = 0;
	while (i < count)
	{
		while (line[start] && ft_ispcnt(line[start]))
			start++;
		end = len_token(line, start);
		tokens[i] = ft_substr(line, start, end - start);
		if (!tokens[i])
		{	
			ft_free_mem(tokens, i);
			return ;
		}
		i++;
		start = end;
		end = 0;
	}
}

static int	count_token(char *line)
{
	int		i;
	int		count;

	i = 0;
	count = 0;
	while (line[i])
	{
		i = len_token(line, i);
		if (line[i] && ft_ispcnt(line[i]))
		{
			count++;
			while (line[i] && ft_ispcnt(line[i]))
				i++;
			if (!line[i])
				count--;
			i--;
		}
		if (line[i])
			i++;
	}
	if (!ft_ispcnt(line[0]) && line[0])
		count++;
	return (count);
}

/* split a string into single tokens. A Token is separeted by */
/* space/tabnewline does not consider space/tab inside quotes */
char	**raw_token_split(char *line, int *exit_n)
{
	char	**tokens;
	int		token_n;

	token_n = count_token(line);
	if (!token_n)
		return (NULL);
	tokens = malloc ((token_n + 1) * sizeof(char *));
	if (!tokens)
	{
		malloc_error(exit_n);
		return (NULL);
	}
	tokens[token_n] = NULL;
	create_arr_arr(line, tokens, token_n);
	if (!tokens)
		malloc_error(exit_n);
	return (tokens);
}