/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_token_processing.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/04 15:37:37 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/10/04 15:37:37 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "executor.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>

/* 
	This function opens a file with the name as in node->content. It then sets 
	the tools->input_fd to the fd of the opened file. The function returns a 
	pointer to the next node.
 */
t_token_lst	*process_input_redir1(t_cmd_tools *tools, t_token_lst *node)
{
	char	*filename;

	filename = node->next->content;
	if (filename[0] == '$')
	{
		print_error_message(REDIR_ERROR, filename);
		tools->input_fd = -1;
	}
	else
	{
		tools->input_fd = open(filename, O_RDONLY | O_CLOEXEC);
		if (tools->input_fd < 0)
			print_error_message(errno, filename);
	}
	return (node->next->next);
}

/* 
	This function sets the tools->input_fd to the fd of the read end of the pipe 
	of the corresponding node of the heredoc linked list. It returns a 
	pointer to the next input node.
 */
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

/* 
	This function opens a file with name as in the node->content, truncates it 
	if it already exists and returns a pointer to the next node.
 */
t_token_lst	*process_output_redir1(t_cmd_tools *tools, t_token_lst *node)
{
	char	*filename;

	filename = node->next->content;
	if (filename[0] == '$')
	{
		print_error_message(REDIR_ERROR, filename);
		tools->input_fd = -1;
	}
	else
	{
		tools->output_fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
		if (tools->input_fd < 0)
			print_error_message(errno, filename);
	}
	return (node->next->next);
}

/* 
	This function opens a file with name as in the node->content in append mode 
	and returns a pointer to the next node.
 */
t_token_lst	*process_output_redir2(t_cmd_tools *tools, t_token_lst *node)
{
	char	*filename;

	filename = node->next->content;
	if (filename[0] == '$')
	{
		print_error_message(REDIR_ERROR, filename);
		tools->input_fd = -1;
	}
	else
	{
		tools->output_fd = open(filename, O_RDWR | O_CREAT | O_APPEND, 0644);
		if (tools->input_fd < 0)
			print_error_message(errno, filename);
	}
	return (node->next->next);
}

/* 
	This function adds the content of the node passed as an argument to the end 
	of the 2D char array tools->cmd_args and returns a pointer to the next node.
 */
t_token_lst	*process_word(t_cmd_tools *tools, t_token_lst *node)
{
	int	i;

	if (node->token_type == EMPTY)
	{
		free (node->content);
		node->content = ft_strdup("");
	}
	i = 0;
	while (tools->cmd_args[i])
		i++;
	tools->cmd_args[i] = node->content;
	return (node->next);
}
