#include "../includes/malloc.h"
#include <stdio.h>
#include <stdlib.h>

struct ovrl_block *g_dma = NULL;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static void init_tiny(void)
{
    g_dma->tiny = (struct t_block *)(g_dma + 1);
    g_dma->tiny->blck_limit = g_dma->tiny + 2;
    g_dma->tiny_limit = (size_t)T_LIMIT;

    g_dma->tiny->next = NULL;
    g_dma->tiny->size = 0;
    g_dma->tiny->inuse = 0;

    g_dma->addr.tiny_start = (char*)g_dma->tiny;
}

static void init_small(void)
{
    g_dma->small = (struct s_block *)((char *)g_dma->tiny + g_dma->tiny_limit);
    g_dma->small->blck_limit = g_dma->small + 1;
    g_dma->small_limit = (size_t)S_LIMIT;

    g_dma->small->next = NULL;
    g_dma->small->size = 0;
    g_dma->small->inuse = 0;

    g_dma->addr.small_start = (char*)g_dma->small;
}

void init_memory(void)
{
  	if (!g_dma)
	{
	  if ((g_dma = mmap(NULL, HDR_SIZE
                              + PREALOC_SIZE_TINY
                              + PREALOC_SIZE_SMALL,
                              MMAP_FLAGS, OFFSET, 0)) == MAP_FAILED)
	    	exit(0);

	  	init_tiny();
		init_small();
		g_dma->get_tiny = push_tiny_chunk;
        g_dma->get_small = push_small_chunk;
        //g_dma->get_large = add_large;
       // g_dma->get_small = get_small;
	} 
	  //init_large();
}

int main(void)
{
  //   init_memory();
  char *ptr = mallok(20);
  mallok(21);
  mallok(24);
  mallok(23);
  show_alloc_mem();
  return 0;
}
