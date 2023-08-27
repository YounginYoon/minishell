/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: youyoon <youyoon@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 19:57:43 by youyoon           #+#    #+#             */
/*   Updated: 2023/08/26 20:00:00 by youyoon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/parse.h"

static int	count_word(char *str)
{
	int	cnt;

	cnt = 0;
	while (*str)
	{
		while (*str == ' ' || (9 <= *str && *str <= 13))
			str++;
		if (*str && !(*str == ' ' || (9 <= *str && *str <= 13)))
		{
			cnt++;
			while (*str && !(*str == ' ' || (9 <= *str && *str <= 13)))
				str++;
		}
	}
	return (cnt);
}

static void	set_quote(char c, t_parse *parse)
{
	parse->quote = 0;
	if (c == '\'')
		parse->buff[parse->b_idx++] = c;
}

void	put_buff_to_cmd(t_parse *parse)
{
	if (!(parse->buff[0]))
		return ;
	parse->cmd[parse->c_idx] = ft_strdup(parse->buff);
	parse->cmd[parse->c_idx + 1] = NULL;
	parse->c_idx++;
	ft_bzero(parse->buff, ft_strlen(parse->buff) + 1);
	parse->b_idx = 0;
}

/*
	SUCCESS : 1
	ERROR : -1
*/

int	parse_char(t_double_list *list, t_parse *parse, char *input, int *i)
{
	int	ret;

	ret = SUCCESS;
	if (parse->quote == input[*i])
		set_quote(input[*i], parse);
	else if (!parse->quote && input[*i] == '\'')
	{
		parse->quote = input[*i];
		parse->buff[parse->b_idx++] = input[*i];
	}
	else if (!parse->quote && input[*i] == '\"')
		parse->quote = input[*i];
	else if (!parse->quote && input[*i] == ' ')
		put_buff_to_cmd(parse);
	else if (!parse->quote && input[*i] == ';')
		ret = ERROR;
	else if (!parse->quote && input[*i] == '|')
	{	
		ret = add_node(list, parse);
		if (ret > 0)
			list->tail->pipe_type = RW_PIPE;
	}
	else if (!parse->quote && ft_strchr("><", input[*i]))
	{
		if (parse->cmd[0] && parse->cmd[0][0] != 0)
			ret = add_node(list, parse);
		parse->redir_type = check_redir(input[*i], input[*i + 1]);
		if (parse->redir_type == A_REDIR || parse->redir_type == H_REDIR)
			(*i)++;
	}
	else
	{
		// if (parse->quote != '\'' && input[*i] == '\\' && input[*i])
		// 	(*i)++;
		parse->buff[parse->b_idx++] = input[*i];
	}
	return (ret);
}

void	parser(char *input, char **envp,  t_double_list *list, t_parse *parse)
{
	int				i;
	int				token_cnt;

	input = ft_strtrim(input, " ");
	token_cnt = count_word(input);
	init_list(list);
	init_parse(parse, token_cnt, (int) ft_strlen(input), envp);
	i = -1;
	while (input[++i])
	{
		if (parse_char(list, parse, input, &i) < 0)
			return (parse_error(list, parse, NULL, SYNTAX_ERROR));
	}
	if (parse->buff[0])
		put_buff_to_cmd(parse);
	if (parse->quote != 0)
		return (parse_error(list, parse, NULL, SYNTAX_ERROR));
	if (add_node(list, parse) < 0)
		return (parse_error(list, parse, NULL, SYNTAX_ERROR));
	set_pipe_type(list);
	set_list_idx(list);
	//parse_error(list, parse, NULL, NULL);
}

		
// /* 출력 테스트 */
// t_node *cur = list->head;
// while (cur)
// {
// 	int i = 0;
// 	printf("|");
// 	while (cur->cmd_args[i])
// 	{	printf(" %s, ", cur->cmd_args[i]); i++; }
// 	printf(" red %d pipe %d idx %d | -> ", cur->redir_type, cur->pipe_type, cur->idx);
// 	cur = cur->next;
// }
// printf("\n");
// /* **** */
// printf("print over\n");