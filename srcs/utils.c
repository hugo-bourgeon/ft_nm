/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hubourge <hubourge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 14:34:27 by hubourge          #+#    #+#             */
/*   Updated: 2025/02/20 20:50:52 by hubourge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

void	init_data(t_data *data, int opt_status)
{
	data->fd = -1;
	data->mapped_file = NULL;
	data->header64 = NULL;
	data->header32 = NULL;
	if (opt_status == DO_INIT_OPT)
	{
		data->nb_files = 0;
		data->opt_g = 0;
		data->opt_u = 0;
		data->opt_r = 0;
		data->opt_p = 0;
		data->opt_h = 0;
	}
}

void	free_all_exit(t_data data, int exit_status)
{
	if (data.mapped_file)
		munmap(data.mapped_file, data.statbuf.st_size);
	if (data.fd != -1)
		close(data.fd);
	if (exit_status == EXIT_FAILURE || exit_status == EXIT_SUCCESS)
		exit(EXIT_FAILURE);
}

void	print_values(int size, int is_undef, long unsigned int address, char type, char *name)
{
	if (type == '0')
		return;
	if (is_undef)
	{
		for (int i = 0; i < size; i++)
			ft_putchar_fd(' ', 1);
		ft_printf(STDOUT_FILENO, " %c %s\n", type, name);
	}
	else
	{
		for(int i = ft_addr_len(address, 16); i < size; i++)
			ft_putchar_fd('0', 1);
		ft_putnbr_base_fd(address, "0123456789abcdef", STDOUT_FILENO);
		ft_printf(STDOUT_FILENO, " %c %s\n",  type, name);
	}
}

int	ft_strcasecmp_nm(const char *s1, const char *s2)
{
	int priority_s1[2];
	int priority_s2[2];

	priority_s1[0] = 0;
	priority_s1[1] = 0;
	priority_s2[0] = 0;
	priority_s2[1] = 0;

	while (*s1 && *s1 == '_')
	{
		priority_s1[0]++;
		s1++;
	}
	while (*s2 && *s2 == '_')
	{
		priority_s2[0]++;
		s2++;
	}

	while (*s1 && *s2)
	{
		while(!ft_isalnum(*s1) && *s1)
		{
			if (*s1 == '_')
				priority_s1[1]++;
			s1++;
		}
		while(!ft_isalnum(*s2) && *s2)
		{
			if (*s2 == '_')
				priority_s2[1]++;
			s2++;
		}
		if (ft_tolower(*s1) != ft_tolower(*s2))
			return (ft_tolower(*s1) - ft_tolower(*s2));
		s1++;
		s2++;
	}
	
	if (ft_tolower(*s1) - ft_tolower(*s2) == 0)
	{
		if (priority_s1[0] != priority_s2[0])
			return (priority_s2[0] - priority_s1[0] );
		if (priority_s1[1] != priority_s2[1])
			return (priority_s2[1] - priority_s1[1]);
	}
	return (ft_tolower(*s1) - ft_tolower(*s2));
}
