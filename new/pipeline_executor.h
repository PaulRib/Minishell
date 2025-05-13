/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_executor.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 17:53:28 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/13 17:53:29 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPELINE_EXECUTOR_H
# define PIPELINE_EXECUTOR_H

# include "../includes/minishell.h"

// Main function to execute a pipeline of commands.
// This is called by execute_commands_sequence_child if pipes are present.
// It will fork for each command in the pipeline.
// Adapted from original execute_pipe in exec_fd.c
void	execute_pipeline(t_shell *shell);

// Handles the execution of a single command within a pipeline (forks).
// Adapted from original child_process in exec.c
void	execute_pipeline_command(t_shell *shell, char *cmd_str, int proc_index, int cmd_in_proc_index);

// Helper to manage file descriptors for pipeline commands
void	setup_pipe_fds(t_shell *shell, int proc_index, int cmd_in_proc_index);

// Helper to wait for a single pipeline command process
void	wait_for_pipeline_command(pid_t pid, t_shell *shell, int proc_index, char *cmd_str);

#endif

