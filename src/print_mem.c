#include "../includes/malloc.h"

long print_alloc(struct t_block *node)
{
    long size;

    printf("%lx", (long)(node + 1));
    printf(" - ");
    printf("%lx", (long)(node->blck_limit));
    printf(" : ");
    size = node->size;//(char*)node->blck_limit - (char*)node - sizeof(struct t_block);
    printf("%ld ", size);
    puts(" octets\n");
    return (size);
}

long print_malloc(struct t_block *node)
{
    long total;

    total = 0;
    while (node->next)
    {
        total += print_alloc(node);
        node = node->next;
    }
    return (total);
}

void show_alloc_mem(void)
{
    pthread_mutex_lock(&mutex);
    if (!g_dma)
    {
        printf("Call show alloc_mem on unitialized dma\n");
        return;
    }
    printf("TINY: %p\n", g_dma->tiny);
    printf("TINY addr : %p\n", g_dma->addr.tiny_start);
    print_malloc(g_dma->tiny);
    printf("SMALL: %p\n", g_dma->small);
    printf("SMALL addr: %p\n", g_dma->addr.small_start);
    pthread_mutex_unlock(&mutex);
}