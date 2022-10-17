/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc_utils.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/04 18:47:14 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/10/04 18:47:14 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"
#include "libft.h"
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <readline/readline.h>
#include <sys/wait.h>

/* 
	This function opens a separate pipe for each node in the here_doc linked list.
	It returns 0 upon success or 1 if the pipe function returns error.
 */
int	heredoc_open_pipes(t_heredoc *hd_list)
{
	while (hd_list)
	{
		if (pipe(hd_list->hd_pipe) < 0)
		{
			ft_putendl_fd(strerror(errno), STDERR_FILENO);
			return (EXIT_FAILURE);
		}
		hd_list = hd_list->next;
	}
	return (EXIT_SUCCESS);
}

/* 
	This function closes specific file descriptors associated with the here_doc 
	linked list in the parent process.
 */
void	heredoc_close_pipes(t_heredoc *hd_list, int exit_code)
{
	while (hd_list)
	{
		if (exit_code == EXIT_FAILURE)
			close(hd_list->hd_pipe[R]);
		close(hd_list->hd_pipe[W]);
		hd_list = hd_list->next;
	}
}

/* 
	This function closes a specific file descriptor associated with the here_doc 
	linked list in the child process.
 */
void	heredoc_child_close_pipes(t_heredoc *hd_list, int end)
{
	while (hd_list)
	{
		close(hd_list->hd_pipe[end]);
		hd_list = hd_list->next;
	}
}

/* 
	The function frees the memory allocated for the linked list containing
	information and user input concerning the here_doc redirections
 */
void	cleanup_hd_list(t_heredoc **hd_list)
{
	t_heredoc	*temp;

	while (*hd_list)
	{
		temp = *hd_list;
		*hd_list = (*hd_list)->next;
		free (temp);
	}
}

/* 
	This function prints an error message, closes the open file descriptors of 
	pipes corresponding to the here_doc, and frees memory allocated for the 
	symbol table, the here_doc linked list and exits the child process
 */
void	heredoc_error_and_exit(t_symtab **symtab, t_heredoc **hd_list, \
t_heredoc *hd_node)
{
	ft_putendl_fd(strerror(errno), STDERR_FILENO);
	symtab_erase_and_free(symtab);
	heredoc_child_close_pipes(hd_node, W);
	cleanup_hd_list(hd_list);
	exit(EXIT_FAILURE);
}
