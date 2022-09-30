/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   executor.h                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/08 15:33:56 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/09/30 10:56:00 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include <stdlib.h>
# include "minishell.h"
# include "symtab.h"

typedef struct s_heredoc
{
	int					hd_pipe[2];
	char				*limiter;
	size_t				cmd_no;
	struct s_heredoc	*next;
}				t_heredoc;

typedef struct s_cmd_tools
{
	pid_t		id;
	size_t		cmd;
	size_t		total_cmds;
	int			input_fd;
	int			output_fd;
	char		**cmd_args;
	char		**paths;
	char		**env_var;
	t_heredoc	*heredoc;
	int			builtin_only;
	t_token_lst	*(*process_tokens[6])(struct s_cmd_tools *tools, \
	t_token_lst *input);
}				t_cmd_tools;

# define BEFORE 0
# define AFTER 1

# define NO_ERROR 0
# define CMD_ERROR -1
# define IO_FD_ERROR -2

# define R 0
# define W 1

# define BEFORE 0
# define AFTER 1

/* executor.c */
// int	executor(t_token_lst *input, t_symtab *symtab, struct sigaction *sa);
int			executor(t_token_lst *input, t_symtab *symtab);
/* exec_parent.c */
int			pipe_and_fork(int *id, t_cmd_tools *tools, int (*pipe_end)[2]);
void		close_unnecessary_pipes(t_cmd_tools *tools, int pipe_end[2][2]);
int			parent_exec_builtin(t_cmd_tools *tools, t_token_lst *input, \
t_symtab *symtab);
int			wait_for_last_child(int id, size_t total_cmds);
/* exec_child.c */
void		perform_cmd(t_cmd_tools *tools, t_token_lst *input, \
int pipe_end[2][2], t_symtab *symtab);
/* token_processing.c */
t_token_lst	*process_word(t_cmd_tools *tools, t_token_lst *node);
t_token_lst	*process_input_redir1(t_cmd_tools *tools, t_token_lst *node);
t_token_lst	*process_input_redir2(t_cmd_tools *tools, t_token_lst *node);
t_token_lst	*process_output_redir1(t_cmd_tools *tools, t_token_lst *node);
t_token_lst	*process_output_redir2(t_cmd_tools *tools, t_token_lst *node);
/* get.c */
void		get_args(t_cmd_tools *tools, t_token_lst *input);
int			get_paths(t_symtab *symtab, t_cmd_tools *tools);
int			get_env_var(t_symtab *symtab, t_cmd_tools *tools);
/* heredoc.c */
int			get_heredoc(t_heredoc *hd_list);
int			check_heredoc(t_token_lst *input, t_cmd_tools *tools);
/* heredoc_child.c */
void		heredoc_child_receive_input(t_heredoc *hd_list);
/* exec_errors.c */
void		free_array(char **array);
void		child_error_and_exit(int error_code, t_cmd_tools *tools, \
char *name);
int			print_error_message(int error_code, char *name);
void		cleanup(t_cmd_tools *tools);
/* init.c */
t_cmd_tools	*tools_init(t_token_lst *input, t_symtab *symtab);

#endif