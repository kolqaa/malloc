#include "../includes/malloc.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *mallok(size_t size)
{
	void *chunk;

	chunk = NULL;
	pthread_mutex_lock(&mutex);

	if (g_dma == NULL)
		init_memory();

	if (size <= TINY_MAX)
		chunk = g_dma->get_block[TINY](g_dma->tiny, size, g_dma->tiny_limit);
	else if (size <= SMALL_MAX)
		chunk = g_dma->get_block[SMALL](g_dma->small, size, g_dma->small_limit);
	else
		chunk = g_dma->get_block[LARGE](size);

	pthread_mutex_unlock(&mutex);
	return (chunk);
}
