/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_help.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsimonov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/27 15:58:50 by nsimonov          #+#    #+#             */
/*   Updated: 2018/05/27 16:04:56 by nsimonov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

long			start_print_small(struct s_s_block *node)
{
	long total;

	total = 0;
	while (node->next)
	{
		total += small_allocation_print(node->next);
		node = node->next;
	}
	return (total);
}

long			large_allocation_print(struct s_l_block *node)
{
	long size;

	if (node == NULL)
		return (0);
	print_hex((long)(node + 1), 1);
	write(1, " - ", 3);
	print_hex((long)(node->blck_limit), 1);
	write(1, " : ", 3);
	size = node->size;
	ft_putnbr(size);
	write(1, " octets\n", 8);
	return (size);
}

long			start_print_large(struct s_l_block *node)
{
	long total;

	total = 0;
	while (node->next)
	{
		total += large_allocation_print(node->next);
		node = node->next;
	}
	return (total);
}

long			tiny_allocation_print(struct s_t_block *node)
{
	long size;

	if (node == NULL)
		return (0);
	print_hex((long)(node + 1), 1);
	write(1, " - ", 3);
	print_hex((long)(node->blck_limit), 1);
	write(1, " : ", 3);
	size = node->size;
	ft_putnbr(size);
	write(1, " octets\n", 8);
	return (size);
}

long			small_allocation_print(struct s_s_block *node)
{
	long size;

	if (node == NULL)
		return (0);
	print_hex((long)(node + 1), 1);
	write(1, " - ", 3);
	print_hex((long)(node->blck_limit), 1);
	write(1, " : ", 3);
	size = node->size;
	ft_putnbr(size);
	write(1, " octets\n", 8);
	return (size);
}
