/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   token_split.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmonfrin <dmonfrin@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/04 15:07:08 by dmonfrin      #+#    #+#                 */
/*   Updated: 2022/10/17 11:18:13 by dmonfrin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "parser.h"
#include "error.h"
#include "libft.h"

static int	ft_ispcnt(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

static int	st_len_token(char *line, int i)
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

static void	st_create_arr_arr(char *line, char **tokens, int count)
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
		end = st_len_token(line, start);
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

static int	st_count_token(char *line)
{
	int		i;
	int		count;

	i = 0;
	count = 0;
	while (line[i])
	{
		i = st_len_token(line, i);
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

/*
    It splits a string into single tokens. A Token is separated by space, tab
    or newline. Does not consider space/tab inside quotes.
*/
char	**raw_token_split(char *line, int *exit_n)
{
	char	**tokens;
	int		token_n;

	token_n = st_count_token(line);
	if (!token_n)
		return (NULL);
	tokens = malloc ((token_n + 1) * sizeof(char *));
	if (!tokens)
	{
		malloc_error(exit_n);
		return (NULL);
	}
	tokens[token_n] = NULL;
	st_create_arr_arr(line, tokens, token_n);
	if (!tokens)
		malloc_error(exit_n);
	return (tokens);
}
