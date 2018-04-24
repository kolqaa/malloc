#include "../includes/malloc.h"

static void	free_large(void *ptr)
{
    struct l_block *node;
    struct l_block *tmp;

    node = g_dma->large;
    while (node->next)
    {
        if (ptr == (node->next + HDR_OFFSET))
        {
            tmp = node->next->next;
            if ((munmap(node->next,
						(char *)node->next->blck_limit
                        - (char *)node->next)) < 0)
                return ;
            node->next = tmp;
            return ;
        }
        node = node->next;
    }
}

static void	free_tiny(struct t_block *tiny_head, void *ptr)
{
    struct t_block *tiny_node;

    tiny_node = tiny_head;
    while (tiny_node->next)
    {
        if (ptr == (tiny_node->next + HDR_OFFSET))
        {
            tiny_node->next = tiny_node->next->next;
            return ;
        }
        tiny_node = tiny_node->next;
    }
}

static void	free_small(struct s_block *small_head, void *ptr)
{
    struct s_block *small_node;

    small_node = small_head;
    while (small_node->next)
    {
        if (ptr == (small_node->next + HDR_OFFSET))
        {
            small_node->next = small_node->next->next;
            return ;
        }
        small_node = small_node->next;
    }
}

void		free(void *ptr)
{
    void *addr;

    pthread_mutex_lock(&mutex);
    if (!ptr)
    {
        pthread_mutex_unlock(&mutex);
        return ;
    }
    addr = g_dma->tiny;
    if (addr < ptr && ptr < (addr + PREALOC_SIZE_TINY))
    {
        free_tiny(addr, ptr);
        pthread_mutex_unlock(&mutex);
        return ;
    }
    addr = g_dma->small;
    if (addr < ptr && ptr < (addr + PREALOC_SIZE_SMALL))
    {
        free_small(addr, ptr);
        pthread_mutex_unlock(&mutex);
        return ;
    }
    free_large(ptr);
    pthread_mutex_unlock(&mutex);
}