/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc_help.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsimonov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/27 15:54:46 by nsimonov          #+#    #+#             */
/*   Updated: 2018/05/27 15:55:56 by nsimonov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

void	check_tiny_size(size_t *size, struct s_t_block *tiny_node)
{
	if ((long)size > (char*)tiny_node->next->blck_limit -
			(char *)tiny_node->next)
		*size = (char *)tiny_node->next->blck_limit -
			(char *)tiny_node->next;
}

void	check_small_size(size_t *size, struct s_s_block *small_node)
{
	if ((long)size > (char*)small_node->next->blck_limit -
			(char *)small_node->next)
		*size = (char *)small_node->next->blck_limit -
			(char *)small_node->next;
}

void	check_large_size(size_t *size, struct s_l_block *large_node)
{
	if ((long)size > (char*)large_node->next->blck_limit -
			(char *)large_node->next)
		*size = (char *)large_node->next->blck_limit -
			(char *)large_node->next;
}

void	free(void *ptr)
{
	pthread_mutex_lock(&g_mutex);
	if (!ptr)
	{
		pthread_mutex_unlock(&g_mutex);
		return ;
	}
	if (try_free_tiny(ptr))
	{
		pthread_mutex_unlock(&g_mutex);
		return ;
	}
	if (try_free_small(ptr))
	{
		pthread_mutex_unlock(&g_mutex);
		return ;
	}
	free_large(ptr);
	pthread_mutex_unlock(&g_mutex);
}
