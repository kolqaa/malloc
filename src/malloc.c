#include "../includes/malloc.h"

/**
 * NOTICE:
 * CALL THIS WITH LOCK MUTEX BECAUSE IT RELEASE IT AT THE END
 */
void *push_tiny_chunk(struct t_block *tiny_head, size_t size, size_t max)
{
    struct  t_block *tiny_node;
    struct  t_block *tmp;
    size += sizeof(struct t_block);

    tiny_node = tiny_head;
    FIND_FREED_BLOCK(tiny_node);

    if (CHECK_MEM_OVERFLOW(tiny_node, tiny_head, size, max))
        return (NULL);

    ADD_CHUNK_TO_DMA(TINY, tiny_node, tmp, (struct t_block*));
    pthread_mutex_unlock(&mutex);
    return (tiny_node->next + 1);
}


/**
 * NOTICE:
 * CALL THIS WITH LOCK MUTEX BECAUSE IT RELEASE IT AT THE END
 */
void *push_small_chunk(struct s_block *small_head, size_t size, size_t max)
{
    struct  s_block *small_node;
    struct  s_block *tmp;
    size += sizeof(struct s_block);

    small_node = small_head;
    FIND_FREED_BLOCK(small_node);

    if (CHECK_MEM_OVERFLOW(small_node, small_head, size, max))
        return (NULL);

    ADD_CHUNK_TO_DMA(SMALL, small_node, tmp, (struct s_block*));
    pthread_mutex_unlock(&mutex);
    return (small_node->next + 1);
}


void *mallok(size_t size)
{
    pthread_mutex_lock(&mutex);

    if (g_dma == NULL)
        init_memory();

    if (size <= T_PAGE_SIZE)
        return (g_dma->get_tiny(g_dma->tiny, size, g_dma->tiny_limit));
    else if (size <= S_PAGE_SIZE)
        return (g_dma->get_small(g_dma->small, size, g_dma->small_limit));


    pthread_mutex_unlock(&mutex);
    return NULL;
}