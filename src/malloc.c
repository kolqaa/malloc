#include "../includes/malloc.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *mallok(size_t size)
{
    void *chunk;

    chunk = NULL;
    pthread_mutex_lock(&mutex);

    if (g_dma == NULL)
        init_memory();

    if (size <= (size_t)T_PAGE_SIZE)
        chunk = g_dma->get_tiny(g_dma->tiny, size, g_dma->tiny_limit);
    else if (size <= (size_t)S_PAGE_SIZE)
        chunk = g_dma->get_small(g_dma->small, size, g_dma->small_limit);
    else
        chunk = g_dma->get_large(size);

    pthread_mutex_unlock(&mutex);
    return (chunk);
}