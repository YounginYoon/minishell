/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: youyoon <youyoon@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 13:35:45 by seokklee          #+#    #+#             */
/*   Updated: 2023/08/28 15:33:12 by youyoon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	exec_unset(t_data *data, char *unset);

void	builtin_unset(t_data *data, t_node *node)
{
	if (node->pipe_type != NO_PIPE || !node->cmd_args[1])
		return ;
	else
	{
		if (ft_isdigit(node->cmd_args[1][0]) || node->cmd_args[1][0] == '=')
			error_str_str_code(node, NON_VALID_ID, 1);
		exec_unset(data, node->cmd_args[1]);
	}
	g_exit_status = 0;
}

static void	exec_unset(t_data *data, char *unset)
{
	int		i;
	int		flag;
	char	*tmp;

	flag = 0;
	i = 0;
	while (data->envp[i + 1])
	{
		if (ft_strncmp(data->envp[i], unset, ft_strlen(unset)))
		{
			tmp = data->envp[i];
			data->envp[i] = data->envp[i + 1];
			data->envp[i + 1] = tmp;
			flag = 1;
		}
		i++;
	}
	if (flag)
	{
		free(data->envp[i]);
		data->envp[i] = NULL;
	}
}