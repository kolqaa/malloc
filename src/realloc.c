#include "../includes/malloc.h"
#include <string.h>

static
inline int is_tiny(void *tiny, void *ptr)
{
    return (tiny < ptr && ptr < (tiny + g_dma->tiny_limit)) ? (1) : (0);
}

static
inline int is_small(void *small, void *ptr)
{
	return (small < ptr && ptr < (small + g_dma->small_limit)) ? (1) : (0);
}

void		*realloc(void *ptr, size_t size)
{
	void	*new;

	if (!ptr || !size)
		return (NULL);

	pthread_mutex_lock(&mutex);

	if (is_tiny(g_dma->tiny, ptr))
		new = realloc_tiny(g_dma->tiny, ptr, size);
	else if (is_small(g_dma->small, ptr))
		new = realloc_small(g_dma->small, ptr, size);
	else
		new = realloc_large(g_dma->large, ptr, size);

	pthread_mutex_unlock(&mutex);
	return  new;
}

