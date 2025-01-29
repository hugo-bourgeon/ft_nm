/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 11:14:29 by hubourge          #+#    #+#             */
/*   Updated: 2025/01/29 20:15:39 by hubourge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

void	print_help(void)
{
	ft_putstr_fd("Usage: ./ft_nm [option(s)] [file(s)]\n", 2);
	ft_putstr_fd("	List symbols in [file(s)] (a.out by default).\n", 2);
	ft_putstr_fd("	The options are:\n", 2);
	ft_putstr_fd(" 		-h, --help\tDisplay this information\n", 2);
	ft_putstr_fd("		-a, --debug-syms       Display debugger-only symbols\n", 2);
	ft_putstr_fd("		-g, --extern-only      Display only external symbols\n", 2);
	ft_putstr_fd("		-u, --undefined-only   Display only undefined symbols\n", 2);
	ft_putstr_fd("		-r, --reverse-sort     Reverse the sense of the sort\n", 2);
	ft_putstr_fd("		-p, --no-sort          Do not sort the symbols\n", 2);
}

int	find_str_set_option(char *str, char *to_find, int *opt, int value)
{
	if (ft_strncmp(str, to_find, ft_strlen(to_find) + 1) == 0)
	{
		*opt = value;
		return (1);
	}
	return (0);
}

int	find_char_set_option(char c, char to_find, int *opt, int value)
{
	if (c == to_find)
	{
		*opt = value;
		return (1);
	}
	return (0);
}

int	is_option_set(t_data *data, char *str)
{
	int i = 1;
	
	if (str[i] == '-') 
	{
		i++;
		if (!str[i]) // Ingone --
			return (1);
		if (find_str_set_option(str, "--debug-syms", &data->opt_a, 1) \
			|| find_str_set_option(str, "--extern-only", &data->opt_g, 1) \
			|| find_str_set_option(str, "--undefined-only", &data->opt_u, 1) \
			|| find_str_set_option(str, "--reverse-sort", &data->opt_r, 1) \
			|| find_str_set_option(str, "--no-sort", &data->opt_p, 1) \
			|| find_str_set_option(str, "--help", &data->opt_h, 1))
			return (1);
	}
	else
	{
		while (str[i])
		{
			if (find_char_set_option(str[i], 'a', &data->opt_a, 1) \
				|| find_char_set_option(str[i], 'g', &data->opt_g, 1) \
				|| find_char_set_option(str[i], 'u', &data->opt_u, 1) \
				|| find_char_set_option(str[i], 'r', &data->opt_r, 1) \
				|| find_char_set_option(str[i], 'p', &data->opt_p, 1) \
				|| find_char_set_option(str[i], 'h', &data->opt_h, 1))
				i++;
			else
				return (0);
		}
		return (1);
	}
	return (0);
}

void	parsing(int ac, char **av, t_data *data)
{
	(void)ac;
	(void)av;
	(void)data;
	int i;

	// Parse options
	i = 1;
	while (av[i])
	{
		if (av[i][0] == '-')
		{
			if (is_option_set(data, av[i]) == 0)
			{
				ft_putstr_fd("ft_nm: unrecognized option ", 2);
				ft_putstr_fd(av[i], 2);
				ft_putstr_fd("\n", 2);
				print_help();
				exit(EXIT_FAILURE);
			}
		}
		i++;
	}
	
	// Parse -h option
	if (data->opt_h == 1)
	{
		print_help();
		exit(EXIT_SUCCESS);
	}

	// Parse files	
}
