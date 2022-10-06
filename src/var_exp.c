/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   var_exp.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmonfrin <dmonfrin@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/04 15:09:02 by dmonfrin      #+#    #+#                 */
/*   Updated: 2022/10/06 12:20:42 by dmonfrin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

/* ************************************************************************** */
/*                                                                            */
/* This function looks for the variable in the symtab and returns an allocate */
/* string with the content of the variable or an allocated empty string '\0'  */
/*                                                                            */
/* ************************************************************************** */
static char	*var_search(char *variable, int sz, t_symtab *symtab)
{
	int		size;
	char	*key;

	if (!symtab)
		return (ft_strdup(""));
	key = ft_substr(variable, 0, sz);
	if (!key)
		return (NULL);
	size = sz + 1;
	while (symtab)
	{
		if (ft_strncmp(key, symtab->key, size) == 0)
		{
			free(key);
			if (symtab->value)
				return (ft_strdup(symtab->value));
			return (ft_strdup(""));
		}
		symtab = symtab->next;
	}
	free(key);
	return (ft_strdup(""));
}

/* ************************************************************************** */
/*                                                                            */
/* if the state is st_word and the variable is not inside the list or         */
/* contains just spaces it returns an allocates string with the variable      */
/* name ex: $not_exist                                                        */
/*                                                                            */
/* ************************************************************************** */
char	*var_opening(char *var, t_state_t state, t_symtab *symtab)
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
	if ((!*content || isallsp(content)) && state == st_word)
	{
		free_set_null(content);
		return (ft_substr(var, 0, i + 1));
	}
	return (content);
}

static char	*word_var_exp(char *exp_str, char *str, int i, t_symtab *symtab)
{
	char	*var;
	char	*trans_var;
	char	*new_str;

	var = var_opening(str + i, st_word, symtab);
	if (!var)
		return (NULL);
	trans_var = var_fixing(var);
	free(var);
	if (!trans_var)
		return (NULL);
	new_str = save_previus(exp_str, str, i);
	if (!new_str)
		return (trans_var);
	return (ft_strjoinfree(new_str, trans_var));
}

static char	*quote_var_exp(char *exp_str, char *str, int i, t_symtab *symtab)
{
	char	*var;

	if (str[i] == '\'')
		return (save_previus(exp_str, str, i + quote_end_set(str, &i)));
	exp_str = save_previus(exp_str, str, i + 1 );
	if (!exp_str)
		return (NULL);
	str += i + 1;
	i = 0;
	while (str[i] && str[i] != '"')
	{
		if (str[i] == '$' && (ft_isvar(str[i + 1]) && !ft_isdigit(str[i + 1])))
		{
			var = var_opening(str + i, st_d_quote, symtab);
			if (!var)
				return (NULL);
			exp_str = save_previus(exp_str, str, i);
			exp_str = ft_strjoinfree(exp_str, var);
			if (!exp_str)
				return (NULL);
			str += var_end_set(str, &i);
		}
		i++;
	}
	if (str[i] == '"')
		i++;
	return (save_previus(exp_str, str, i));
}

/* ************************************************************************** */
/*                                                                            */
/* If there are variables inside the input they are going to be expanded:     */
/* if the variable is inside the double quotes is gonna be expanded how it is */
/* if it is alone it's going to be considered as a token or group of token    */
/* and to avoid keywords such as > or | to consider keyword we include each   */
/* token inside double quote es:                                              */
/* word="    ls    -a  "                                                      */
/* `hello $word`                                                              */
/* `hello  "ls" "-a"                                                          */
/*                                                                            */
/* ************************************************************************** */
char	*var_expantion(char *str, t_symtab *symtab)
{
	int		i;
	char	*exp_str;

	i = 0;
	exp_str = NULL;
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			exp_str = quote_var_exp(exp_str, str, i, symtab);
			if (!exp_str)
				return (NULL);
			str += quote_end_set(str, &i);
		}
		else if (str[i] == '$' && (ft_isvar(str[i + 1])
				&& !ft_isdigit(str[i + 1])))
		{
			exp_str = word_var_exp(exp_str, str, i, symtab);
			if (!exp_str)
				return (NULL);
			str += var_end_set(str, &i);
		}
		i++;
	}
	return (save_previus(exp_str, str, i));
}
