/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block_reallocation.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsimonov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/20 15:09:16 by nsimonov          #+#    #+#             */
/*   Updated: 2018/05/27 15:57:18 by nsimonov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	char *tmp;

	tmp = dest;
	while (n--)
		*tmp++ = *(unsigned char*)src++;
	return (dest);
}

void	*realloc_tiny(struct t_block *tiny_head, void *ptr, size_t size)
{
	struct t_block	*tiny_node;

	tiny_node = tiny_head;
	while (tiny_node->next)
	{
		if (ptr == (tiny_node->next + 1))
		{
			pthread_mutex_unlock(&g_mutex);
			if (!(ptr = malloc(size)))
			{
				pthread_mutex_lock(&g_mutex);
				return (NULL);
			}
			pthread_mutex_lock(&g_mutex);
			check_tiny_size(&size, tiny_node);
			ft_memcpy(ptr, tiny_node->next + 1, size);
			pthread_mutex_unlock(&g_mutex);
			free(tiny_node->next);
			pthread_mutex_lock(&g_mutex);
			return (ptr);
		}
		tiny_node = tiny_node->next;
	}
	return (NULL);
}

void	*realloc_small(struct s_block *small_head, void *ptr, size_t size)
{
	struct s_block	*small_node;

	small_node = small_head;
	while (small_node->next)
	{
		if (ptr == (small_node->next + 1))
		{
			pthread_mutex_unlock(&g_mutex);
			if (!(ptr = malloc(size)))
			{
				pthread_mutex_lock(&g_mutex);
				return (NULL);
			}
			pthread_mutex_lock(&g_mutex);
			check_small_size(&size, small_node);
			ft_memcpy(ptr, small_node->next + 1, size);
			pthread_mutex_unlock(&g_mutex);
			free(small_node->next);
			pthread_mutex_lock(&g_mutex);
			return (ptr);
		}
		small_node = small_node->next;
	}
	return (NULL);
}

void	*realloc_large(struct l_block *large_head, void *ptr, size_t size)
{
	struct l_block	*large_node;

	large_node = large_head;
	while (large_node->next)
	{
		if (ptr == (large_node->next + 1))
		{
			pthread_mutex_unlock(&g_mutex);
			if (!(ptr = malloc(size)))
			{
				pthread_mutex_lock(&g_mutex);
				return (NULL);
			}
			pthread_mutex_lock(&g_mutex);
			check_large_size(&size, large_node);
			ft_memcpy(ptr, large_node->next + 1, size);
			pthread_mutex_unlock(&g_mutex);
			free(large_node->next);
			pthread_mutex_lock(&g_mutex);
			return (ptr);
		}
		large_node = large_node->next;
	}
	return (NULL);
}
