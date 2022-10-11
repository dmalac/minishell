/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/12 12:33:23 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/09/20 16:39:15 by dmalacov      ########   odam.nl         */
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
	This function creates a new node of the heredoc linked list, adds the values 
	cmd_no and content and returns a pointer to the node.
 */
static t_heredoc	*st_heredoc_new(t_token_lst *node, size_t cmd_no)
{
	t_heredoc	*new;

	new = malloc(sizeof(t_heredoc));
	if (!new)
		return (ft_putendl_fd(strerror(errno), STDERR_FILENO), NULL);
	new->cmd_no = cmd_no;
	if (node->content[0] == '"' || node->content[0] == '\'')
	{
		new->limiter = ft_substr(node->content, 1, \
		ft_strlen(node->content) - 2);
		free (node->content);
		node->content = new->limiter;
		new->expand = 0;
	}
	else
	{
		new->limiter = node->content;
		new->expand = 1;
	}
	new->next = NULL;
	return (new);
}

/* 
	This function adds a new node to the back of the heredoc linked list.
 */
static void	st_heredoc_add_back(t_heredoc **top, t_heredoc *new)
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

/* 
	This function opens one pipe for each "<<" redirection sign in the input and 
	creates a child process to retrieve the user input. It returns 0 upon 
	success and 1 if an error occurs or the child process exits because of the 
	SIGINT signal.
 */
int	get_heredoc(t_heredoc *hd_list, t_symtab *symtab)
{
	pid_t			id;
	int				exit_code;
	int				wait_status;

	exit_code = EXIT_SUCCESS;
	if (!hd_list)
		return (EXIT_SUCCESS);
	if (heredoc_open_pipes(hd_list) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	id = fork();
	if (id < 0)
		return (ft_putendl_fd(strerror(errno), STDERR_FILENO), 1);
	if (id == 0)
		heredoc_child_process_redir(&hd_list, &symtab);
	else
	{
		waitpid(id, &wait_status, 0);
		if (WIFEXITED(wait_status))
			exit_code = WEXITSTATUS(wait_status);
		heredoc_close_pipes(hd_list);
	}
	return (exit_code);
}

/* 
	This function checks whether the input line contains a "<<" redirection and 
	if so, it creates a new node in the heredoc linked list for each "<<" 
	redirection. It returns 0 upon success and 1 if an allocation error occurs.
*/
int	check_heredoc(t_token_lst *input, t_cmd_tools *tools)
{
	size_t		cmd_no;
	t_token_lst	*node;
	t_heredoc	*new;

	cmd_no = 1;
	tools->heredoc = NULL;
	node = input;
	while (node)
	{
		if (node->token_type == PIPE)
			cmd_no++;
		if (node->token_type == DSML_TH)
		{
			node = node->next;
			new = st_heredoc_new(node, cmd_no);
			if (!new)
				return (EXIT_FAILURE);
			st_heredoc_add_back(&tools->heredoc, new);
		}
		node = node->next;
	}
	return (EXIT_SUCCESS);
}
