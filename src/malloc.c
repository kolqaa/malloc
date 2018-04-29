#include "../includes/malloc.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *mallok(size_t size)
{
    void *chunk;

    chunk = NULL;
    pthread_mutex_lock(&mutex);

    if (g_dma == NULL)
        init_memory();

    if (size <= (size_t)TINY_MAX)
        chunk = g_dma->get_block[TINY](g_dma->tiny, size, TINY_ZONE);
    else if (size <= (size_t)SMALL_MAX)
        chunk = g_dma->get_block[SMALL](g_dma->small, size, SMALL_ZONE);
    else
        chunk = g_dma->get_block[LARGE](size);

    pthread_mutex_unlock(&mutex);
    return (chunk);
}