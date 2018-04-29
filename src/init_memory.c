#include "../includes/malloc.h"
#include <stdlib.h>

struct ovrl_block *g_dma = NULL;

static void init_tiny(void)
{
    g_dma->tiny = (struct t_block *)(g_dma->large + 1);
    g_dma->tiny->blck_limit = g_dma->tiny + 1;
    g_dma->tiny->next = NULL;

    g_dma->tiny_limit = TINY_ZONE;
    g_dma->addr.tiny_start = (char*)g_dma->tiny;
}

static void init_small(void)
{
    g_dma->small = (struct s_block *)((char *)g_dma->tiny + TINY_ZONE);
    g_dma->small->blck_limit = g_dma->small + 1;
    g_dma->small->next = NULL;

    g_dma->small_limit = SMALL_ZONE;
    g_dma->addr.small_start = (char*)g_dma->small;
}

static void init_large(void)
{
    g_dma->large = (struct l_block*)(g_dma + 1);
    g_dma->large->next = NULL;

    g_dma->addr.large_start = (char *)g_dma->large;
}

void init_memory(void)
{
  	if (!g_dma)
	{
        if ((g_dma = mmap(NULL, HDR_SIZE + TINY_ZONE + SMALL_ZONE, MMAP_FLAGS,
                          OFFSET, 0)) == MAP_FAILED)
            exit(0);

        init_large();
	  	init_tiny();
		init_small();
        g_dma->large->blck_limit = (struct l_block *)((char *)g_dma->small
                                                      + SMALL_ZONE);
        g_dma->get_block[TINY] = push_tiny_chunk;
        g_dma->get_block[SMALL] = push_small_chunk;
        g_dma->get_block[LARGE] = push_large_chunk;

        g_dma->print_block[TINY] = start_print_tiny;
        g_dma->print_block[SMALL] = start_print_small;
        g_dma->print_block[LARGE] = start_print_large;
    }
}

/*#include <string.h>
int main(void)
{
    char *ptr = mallok(2);

    strcpy(ptr, "a");

    show_alloc_mem();


     realloc(ptr, 6890);

    strcpy(ptr, "hello my world");

    show_alloc_mem();

    return 0;

  return 0;
}*/
