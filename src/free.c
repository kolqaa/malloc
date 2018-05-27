/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsimonov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/20 14:56:19 by nsimonov          #+#    #+#             */
/*   Updated: 2018/05/27 15:56:26 by nsimonov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

static void	free_large(void *ptr)
{
	struct l_block *node;
	struct l_block *tmp;

	node = g_dma->large;
	while (node->next)
	{
		if (ptr == (node->next + HDR_OFFSET) || ptr == (node->next))
		{
			tmp = node->next->next;
			if ((munmap(node->next,
							(char *)node->next->blck_limit
							- (char *)node->next)) < 0)
				return ;
			node->next = tmp;
			return ;
		}
		node = node->next;
	}
}

static void	free_tiny(struct t_block *tiny_head, void *ptr)
{
	struct t_block *tiny_node;

	tiny_node = tiny_head;
	while (tiny_node->next)
	{
		if (ptr == (tiny_node->next + HDR_OFFSET) || ptr == (tiny_node->next))
		{
			tiny_node->next = tiny_node->next->next;
			return ;
		}
		tiny_node = tiny_node->next;
	}
}

static void	free_small(struct s_block *small_head, void *ptr)
{
	struct s_block *small_node;

	small_node = small_head;
	while (small_node->next)
	{
		if (ptr == (small_node->next + HDR_OFFSET) || ptr == (small_node->next))
		{
			small_node->next = small_node->next->next;
			return ;
		}
		small_node = small_node->next;
	}
}

int			try_free_tiny(void *ptr)
{
	void *addr;

	addr = g_dma->tiny;
	if (addr < ptr && ptr < (addr + TINY_ZONE))
	{
		free_tiny(addr, ptr);
		return (1);
	}
	return (0);
}

int			try_free_small(void *ptr)
{
	void *addr;

	addr = g_dma->small;
	if (addr < ptr && ptr < (addr + SMALL_ZONE))
	{
		free_small(addr, ptr);
		return (1);
	}
	return (0);
}
