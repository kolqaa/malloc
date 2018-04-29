#include "../includes/malloc.h"


void    *push_tiny_chunk(struct t_block *tiny_head, size_t size, size_t limit)
{
    struct  t_block *tiny_node;
    struct  t_block *tmp;
    size += sizeof(struct t_block);

    tiny_node = tiny_head;
    while (tiny_node->next && IN_PAGE(tiny_node, (long)size))
    {
        tiny_node = tiny_node->next;
    }

    if ((char*)tiny_node->blck_limit + size > (char*)tiny_head + limit)
        return NULL;

    tmp = tiny_node->next;
    tiny_node->next = tiny_node->blck_limit;
    tiny_node->next->blck_limit =
            (struct t_block *)((char *)tiny_node->next + size);
    tiny_node->next->next = tmp;
    tiny_node->next->size = size - sizeof(struct t_block);

    return (tiny_node->next + HDR_OFFSET);
}

void    *push_small_chunk(struct s_block *small_head, size_t size, size_t limit)
{
    struct  s_block *small_node;
    struct  s_block *tmp;
    size += sizeof(struct s_block);

    small_node = small_head;
    while (small_node->next && IN_PAGE(small_node, (long)size))
        small_node = small_node->next;

    if ((char*)small_node->blck_limit + size > (char*)small_head + limit)
        return NULL;

    tmp = small_node->next;
    small_node->next = small_node->blck_limit;
    small_node->next->blck_limit =
            (struct s_block *)((char *)small_node->next + size);
    small_node->next->next = tmp;
    small_node->next->size = size - sizeof(struct s_block);

    return (small_node->next + HDR_OFFSET);
}

void    *push_large_chunk(size_t size)
{
    struct l_block *l_node;
    size += sizeof(struct l_block);

    l_node = g_dma->large;
    while (l_node->next)
        l_node = l_node->next;
    if ((l_node->next = mmap(NULL, size, MMAP_FLAGS, OFFSET, 0)) == MAP_FAILED)
        return (NULL);
    l_node->next->blck_limit = (struct l_block *)((char *)l_node->next + size);
    l_node->next->next = NULL;
    l_node->next->size = size - sizeof(struct l_block);
    return (l_node->next + HDR_OFFSET);
}