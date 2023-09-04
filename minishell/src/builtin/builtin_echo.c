/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seokklee <seokklee@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 13:35:33 by seokklee          #+#    #+#             */
/*   Updated: 2023/09/04 15:02:22 by seokklee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	builtin_echo(t_data *data, t_node *node)
{
	int	new_line;
	int	i;

	new_line = 1;
	i = 1;
	if (node->cmd_args[i] == NULL)
		return (ft_putchar_fd('\n', data->output_fd));
	if (!strncmp(node->cmd_args[i], "-n", ft_strlen(node->cmd_args[i])))
	{
		new_line = 0;
		i++;
	}
	while (node->cmd_args[i])
	{
		if (!strncmp(node->cmd_args[i], "$?", ft_strlen(node->cmd_args[i])))
			ft_putstr_fd(ft_itoa(g_exit_status), data->output_fd);
		else
			ft_putstr_fd(node->cmd_args[i], data->output_fd);
		if (node->cmd_args[i + 1] != NULL)
			ft_putchar_fd(' ', data->output_fd);
		i++;
	}
	if (new_line)
		ft_putchar_fd('\n', data->output_fd);
	g_exit_status = 0;
}
