/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_memory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsimonov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/20 14:42:46 by nsimonov          #+#    #+#             */
/*   Updated: 2018/05/27 16:14:10 by nsimonov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

struct s_block *g_dma = NULL;

static void	init_tiny(void)
{
	g_dma->tiny = (struct s_t_block *)(g_dma->large + 1);
	g_dma->tiny->blck_limit = g_dma->tiny + 1;
	g_dma->tiny->next = NULL;
	g_dma->tiny_limit = TINY_ZONE;
	g_dma->addr.tiny_start = (char*)g_dma->tiny;
}

static void	init_small(void)
{
	g_dma->small = (struct s_s_block *)((char *)g_dma->tiny + TINY_ZONE);
	g_dma->small->blck_limit = g_dma->small + 1;
	g_dma->small->next = NULL;
	g_dma->small_limit = SMALL_ZONE;
	g_dma->addr.small_start = (char*)g_dma->small;
}

static void	init_large(void)
{
	g_dma->large = (struct s_l_block*)(g_dma + 1);
	g_dma->large->next = NULL;
	g_dma->addr.large_start = (char *)g_dma->large;
}

void		init_memory(void)
{
	if (!g_dma)
	{
		if ((g_dma = mmap(NULL, HDR_SIZE + TINY_ZONE + SMALL_ZONE, MMAP_FLAGS,
						OFFSET, 0)) == MAP_FAILED)
			return ;
		init_large();
		init_tiny();
		init_small();
		g_dma->large->blck_limit = (struct s_l_block *)((char *)g_dma->small
				+ SMALL_ZONE);
		g_dma->get_block[TINY] = push_tiny_chunk;
		g_dma->get_block[SMALL] = push_small_chunk;
		g_dma->get_block[LARGE] = push_large_chunk;
		g_dma->print_block[TINY] = start_print_tiny;
		g_dma->print_block[SMALL] = start_print_small;
		g_dma->print_block[LARGE] = start_print_large;
	}
}
