/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   token_processing.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/09 14:04:57 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/09/14 17:10:20 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "executor.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>

t_token_lst	*process_input_redir1(t_cmd_tools *tools, t_token_lst *node)
{
	char	*filename;

	filename = node->next->content;
	tools->input_fd = open(filename, O_RDONLY | O_CLOEXEC);
	if (tools->input_fd < 0)
		print_error_message(errno, filename);
	return (node->next->next);
}

t_token_lst	*process_input_redir2(t_cmd_tools *tools, t_token_lst *node)
{
	t_heredoc	*hd_node;

	hd_node = tools->heredoc;
	while (hd_node && ft_strncmp(hd_node->limiter, node->next->content, \
	ft_strlen(hd_node->limiter) + 1) != 0)
		hd_node = hd_node->next;
	if (hd_node)
		tools->input_fd = hd_node->hd_pipe[R];
	return (node->next->next);
}

t_token_lst	*process_output_redir1(t_cmd_tools *tools, t_token_lst *node)
{
	char	*filename;

	filename = node->next->content;
	tools->output_fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (tools->input_fd < 0)
		print_error_message(errno, filename);
	return (node->next->next);
}

t_token_lst	*process_output_redir2(t_cmd_tools *tools, t_token_lst *node)
{
	char	*filename;

	filename = node->next->content;
	tools->output_fd = open(filename, O_RDWR | O_CREAT | O_APPEND, 0644);
	if (tools->input_fd < 0)
		print_error_message(errno, filename);
	return (node->next->next);
}

t_token_lst	*process_word(t_cmd_tools *tools, t_token_lst *node)
{
	int	i;

	if (node->token_type == WORD)
	{
		i = 0;
		while (tools->cmd_args[i])
			i++;
		tools->cmd_args[i] = node->content;
	}
	return (node->next);
}
