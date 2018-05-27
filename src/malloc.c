/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsimonov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/20 14:38:35 by nsimonov          #+#    #+#             */
/*   Updated: 2018/05/20 15:55:02 by nsimonov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;

void	*malloc(size_t size)
{
	void *chunk;

	chunk = NULL;
	pthread_mutex_lock(&g_mutex);
	if (g_dma == NULL)
		init_memory();
	if (size <= TINY_MAX)
		chunk = g_dma->get_block[TINY](g_dma->tiny, size, g_dma->tiny_limit);
	else if (size <= SMALL_MAX)
		chunk = g_dma->get_block[SMALL](g_dma->small, size, g_dma->small_limit);
	else
		chunk = g_dma->get_block[LARGE](size);
	pthread_mutex_unlock(&g_mutex);
	return (chunk);
}
