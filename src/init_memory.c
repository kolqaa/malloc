#include "../includes/malloc.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

struct ovrl_block *g_dma = NULL;

static void init_tiny(void)
{
    g_dma->tiny = (struct t_block *)(g_dma + 1);
    g_dma->tiny->blck_limit = g_dma->tiny + 2;
    g_dma->tiny_limit = T_LIMIT;

    g_dma->tiny->next = NULL;
    g_dma->tiny->size = 0;
    g_dma->tiny->inuse = 0;

    g_dma->addr.tiny_start = (char*)g_dma->tiny;
}

static void init_small(void)
{
    g_dma->small = (struct s_block *)((char *)g_dma->tiny + g_dma->tiny_limit);
    g_dma->small->blck_limit = g_dma->small + 1;
    g_dma->small_limit = S_LIMIT;

    g_dma->small->next = NULL;
    g_dma->small->size = 0;
    g_dma->small->inuse = 0;

    g_dma->addr.small_start = (char*)g_dma->small;
}



void show_alloc_mem(void)
{
    printf("TINY: %p\n", g_dma->tiny);
    printf("SMALL: %p\n", g_dma->small);
}

/*static void init_large(void)
{
  	g_dma->l = NULL;
	g_dma->l->size = 0;
	g_dma->l->inuse = 0;
	g_dma->l->next = NULL;
    }*/

void init_memory(void)
{
	const size_t SIZE = (sizeof(struct ovrl_block) + T_PAGE_SIZE + S_PAGE_SIZE) * 200;
  	if (!g_dma)
	{
		if ((g_dma = mmap(NULL, SIZE, MMAP_FLAGS, OFFSET, 0)) == MAP_FAILED)
			exit(0);
		init_tiny();
        init_small();
    }
	  //init_large();
}


int main(void)
{
    init_memory();
    show_alloc_mem();

    return 0;
}
