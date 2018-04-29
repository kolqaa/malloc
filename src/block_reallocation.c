#include "../includes/malloc.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
    char *tmp;

    tmp = dest;
    while (n--)
        *tmp++ = *(unsigned char*)src++;
    return (dest);
}

/**
 *  CAUTION! call it with mutex held,
 *  this function release it.
 */
void	*realloc_tiny(struct t_block *tiny_head, void *ptr, size_t size)
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

            ft_memcpy(ptr, tiny_node->next + 1, size);

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
void	*realloc_small(struct s_block *small_head, void *ptr, size_t size)
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

            ft_memcpy(ptr, small_node->next + 1, size);

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
void	*realloc_large(struct l_block *large_head, void *ptr, size_t size)
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

            ft_memcpy(ptr, large_node->next + 1, size);

            pthread_mutex_unlock(&mutex);
            free(large_node->next);
            pthread_mutex_lock(&mutex);
            return (ptr);
        }
        large_node = large_node->next;
    }

    return (NULL);
}