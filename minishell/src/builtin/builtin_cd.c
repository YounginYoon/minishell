/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: youyoon <youyoon@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 13:35:31 by seokklee          #+#    #+#             */
/*   Updated: 2023/08/28 14:43:27 by youyoon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	builtin_cd_home(t_data *data, t_node *node);
static void	set_pwds(t_data *data);
static char	*get_envv(char *envp[], char *envv);

void	builtin_cd(t_data *data, t_node *node)
{
	char	*path;

	path = NULL;
	if (node->cmd_args[1] == NULL \
	|| !ft_strncmp(node->cmd_args[1], "~", ft_strlen(node->cmd_args[1])))
		builtin_cd_home(data, node);
	else
	{
		path = node->cmd_args[1];
		if (chdir(path) < 0)
			error_str_str_code(node, NO_FILE_DIR, 1);
		set_pwds(data);
	}
	g_exit_status = 0;
}

static void	builtin_cd_home(t_data *data, t_node *node)
{
	char	*home;

	home = get_envv(data->envp, "HOME");
	if (chdir(home) < 0)
	{
		error_str_code(node, HOME_NOT_SET, 1);
		exit(g_exit_status);
	}
	set_pwds(data);
}

void	set_pwds(t_data *data)
{
	char	*pwd;
	char	*oldpwd;
	char	*tmp;

	tmp = (char *)malloc(sizeof(char) * 2048);
	pwd = ft_strjoin("PWD=", getcwd(tmp, 2048));
	oldpwd = ft_strjoin("OLDPWD=", get_envv(data->envp, "PWD"));
	exec_export(data, pwd);
	exec_export(data, oldpwd);
	free(tmp);
	free(pwd);
	free(oldpwd);
}

char	*get_envv(char *envp[], char *envv)
{
	while (*envp)
	{
		if (!ft_strncmp(*envp, envv, ft_strlen(envv)))
			return (*envp + ft_strlen(envv) + 1);
		envp++;
	}
	return ("");
}