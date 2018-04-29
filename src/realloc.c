#include "../includes/malloc.h"
#include <string.h>


static
inline int is_tiny(void *tiny, void *ptr)
{
    return (tiny < ptr && ptr < (tiny + TINY_ZONE)) ? (1) : (0);
}

static
inline int is_small(void *small, void *ptr)
{
    return (small < ptr && ptr < (small + SMALL_ZONE)) ? (1) : (0);
}

/**
 *  CAUTION! call it with mutex held,
 *  this function release it.
 */

static void	*realloc_tiny(struct t_block *tiny_head, void *ptr, size_t size)
{
    struct t_block	*tiny_node;

    tiny_node = tiny_head;
    while (tiny_node->next)
    {
        if (ptr == (tiny_node->next + 1))
        {
            pthread_mutex_unlock(&mutex);
            if (!(ptr = mallok(size)))
            {
                pthread_mutex_lock(&mutex);
                return (NULL);
            }

            pthread_mutex_lock(&mutex);
            if ((long)size > (char*)tiny_node->next->blck_limit -
                                     (char *)tiny_node->next)
                size = (char *)tiny_node->next->blck_limit -
                        (char *)tiny_node->next;

            memcpy(ptr, tiny_node->next + 1, size);

            pthread_mutex_unlock(&mutex);
            free(tiny_node->next);
            pthread_mutex_lock(&mutex);

            return (ptr);
        }
        tiny_node = tiny_node->next;
    }
    return (NULL);
}


/**
*  CAUTION! call it with mutex held,
*  this function release it.
*/

static void	*realloc_small(struct s_block *small_head, void *ptr, size_t size)
{
    struct s_block	*small_node;

    small_node = small_head;
    while (small_node->next)
    {
        if (ptr == (small_node->next + 1))
        {
            pthread_mutex_unlock(&mutex);
            if (!(ptr = mallok(size)))
            {
                pthread_mutex_lock(&mutex);
                return (NULL);
            }

            pthread_mutex_lock(&mutex);
            if ((long)size > (char*)small_node->next->blck_limit -
                                     (char *)small_node->next)
                size = (char *)small_node->next->blck_limit -
                        (char *)small_node->next;

            memcpy(ptr, small_node->next + 1, size);

            pthread_mutex_unlock(&mutex);
            free(small_node->next);
            pthread_mutex_lock(&mutex);

            return (ptr);
        }
        small_node = small_node->next;
    }

    return (NULL);
}

/**
*  CAUTION! call it with mutex held,
*  this function release it.
*/

static void	*realloc_large(struct l_block *large_head, void *ptr, size_t size)
{
    struct l_block	*large_node;

    large_node = large_head;
    while (large_node->next)
    {
        if (ptr == (large_node->next + 1))
        {
            pthread_mutex_unlock(&mutex);
            if (!(ptr = mallok(size)))
            {
                pthread_mutex_lock(&mutex);
                return (NULL);

            }

            pthread_mutex_lock(&mutex);
            if ((long)size > (char*)large_node->next->blck_limit -
                                     (char *)large_node->next)
                size = (char *)large_node->next->blck_limit -
                        (char *)large_node->next;

            memcpy(ptr, large_node->next + 1, size);

            pthread_mutex_unlock(&mutex);
            free(large_node->next);
            pthread_mutex_lock(&mutex);
            return (ptr);
        }
        large_node = large_node->next;
    }

    return (NULL);
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

