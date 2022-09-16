/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/12 12:33:23 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/09/16 16:15:16 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "main.h"
#include "libft.h"
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <readline/readline.h>
#include <fcntl.h>	// delete

t_heredoc	*st_heredoc_new(t_token_lst *node, size_t cmd_no)
{
	t_heredoc	*new;

	new = malloc(sizeof(t_heredoc));
	if (!new)
		return (NULL);
	new->cmd_no = cmd_no;
	new->limiter = node->content;
	new->next = NULL;
	return (new);
}

void	st_heredoc_add_back(t_heredoc **top, t_heredoc *new)
{
	t_heredoc	*node;

	if (!(*top))
		*top = new;
	else
	{
		node = *top;
		while (node->next)
			node = node->next;
		node->next = new;
	}
}

void	get_next_heredoc(t_heredoc *hd_list)
{
	char	*line;

	while (hd_list)
	{
		if (pipe(hd_list->hd_pipe) < 0)
			// ERR MESS & RETURN, NOT EXIT
			// error_and_exit(errno, tools, input, NULL);
		line = NULL;
		while (!line || ft_strncmp(line, hd_list->limiter, \
		ft_strlen(hd_list->limiter) + 1) != 0)
		{
			if (line)
			{
				write(hd_list->hd_pipe[W], line, ft_strlen(line));
				write(hd_list->hd_pipe[W], "\n", 1);
				// free(line);
				// line = NULL;
			}
			line = readline("> ");
		}
		// free(line);
		// line = NULL;
		close(hd_list->hd_pipe[W]);
		hd_list = hd_list->next;
	}
}

t_heredoc	*check_heredoc(t_token_lst *input)
{
	size_t		cmd_no;
	t_token_lst	*node;
	t_heredoc	*top;
	t_heredoc	*new;

	cmd_no = 1;
	top = NULL;
	node = input;
	while (node)
	{
		if (node->token_type == PIPE)
			cmd_no++;
		if (node->token_type == DSMLR_TH)
		{
			node = node->next;
			new = st_heredoc_new(node, cmd_no);
			if (!new)
			{
				// ERR MESS & RETURN, NOT EXIT
				// error_and_exit(errno, tools, input, NULL);
			}
			st_heredoc_add_back(&top, new);
		}
		node = node->next;
	}
	return (top);
}
