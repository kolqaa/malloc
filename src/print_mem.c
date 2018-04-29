#include "../includes/malloc.h"


long small_allocation_print(struct s_block *node)
{
    long size;

    if (node == NULL)
        return 12;
    printf("%lx", (long)(node + 1));
    printf(" - ");
    printf("%lx", (long)(node->blck_limit));
    printf(" : ");
    size = node->size;
    printf("%ld ", size);
    puts(" octets");
    return (size);
}

long start_print_small(struct s_block *node)
{
    long total;

    total = 0;

    while (node->next)
    {
        total += small_allocation_print(node->next);
        node = node->next;
    }
    return (total);
}


long large_allocation_print(struct l_block *node)
{
    long size;

    if (node == NULL)
        return 12;
    printf("%lx", (long)(node + 1));
    printf(" - ");
    printf("%lx", (long)(node->blck_limit));
    printf(" : ");
    size = node->size;
    printf("%ld ", size);
    puts(" octets");
    return (size);
}

long start_print_large(struct l_block *node)
{
    long total;

    total = 0;

    while (node->next)
    {
        total += large_allocation_print(node->next);
        node = node->next;
    }
    return (total);
}

long tiny_allocation_print(struct t_block *node)
{
    long size;

    if (node == NULL)
        return 12;
    printf("%lx", (long)(node + 1));
    printf(" - ");
    printf("%lx", (long)(node->blck_limit));
    printf(" : ");
    size = node->size;
    printf("%ld ", size);
    puts(" octets");
    return (size);
}

long start_print_tiny(struct t_block *node)
{
    long total;

    total = 0;

    while (node->next)
    {
        total += tiny_allocation_print(node->next);
        node = node->next;
    }
    return (total);
}

void show_alloc_mem(void)
{
    void *ptr;
    pthread_mutex_lock(&mutex);
    if (!g_dma)
    {
        printf("Call show alloc_mem on unitialized dma\n");
        return;
    }
    printf("TINY : %p\n", g_dma->tiny);
    ptr = g_dma->tiny;
    g_dma->print_block[TINY](ptr);
    printf("SMALL : %p\n", g_dma->small);
    ptr = g_dma->small;
    g_dma->print_block[SMALL](ptr);
    printf("LARGE : %p\n", g_dma->large);
    ptr = g_dma->large;
    g_dma->print_block[LARGE](ptr);
    pthread_mutex_unlock(&mutex);
}